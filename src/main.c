#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include "minmea.h"

//NMEA: max 82 chars + nul terminator + margin
#define MSG_SIZE 85

//queue shared between the UART ISR and the processing thread
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

#define GPS_UART_NODE DT_ALIAS(gps_uart)
static const struct device *const gps_dev = DEVICE_DT_GET(GPS_UART_NODE);

static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

void serial_cb(const struct device *dev, void *user_data)
{
    uint8_t c;

    if (!uart_irq_update(dev) || !uart_irq_rx_ready(dev)) {
        return;
    }

    while (uart_fifo_read(dev, &c, 1) == 1) {
        if (c == '\n' || rx_buf_pos >= MSG_SIZE - 1) {
            rx_buf[rx_buf_pos] = '\0';
            k_msgq_put(&uart_msgq, rx_buf, K_NO_WAIT);
            rx_buf_pos = 0;
        } else if (c != '\r') {
            rx_buf[rx_buf_pos++] = c;
        }
    }
}

void gps_processing_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    char line[MSG_SIZE];
    struct minmea_sentence_gga frame;


    while (1) {
        //Block until a sentence arrives
        if (k_msgq_get(&uart_msgq, line, K_FOREVER) != 0) {
            continue;
        }
        //Skip non-GGA
        if (minmea_sentence_id(line, false) != MINMEA_SENTENCE_GGA) {
            continue;
        }
        //Validate the checksu
        if (!minmea_parse_gga(&frame, line)) {
            continue;
        }
        //Checking quality
        if (frame.fix_quality == 0) {
            continue;
        }
        //printk does not support %f
        printk("Latitude: %d.%06d\n",
               (int)minmea_tocoord(&frame.latitude),
               (int)(minmea_tocoord(&frame.latitude) * 1e6) % 1000000);
        printk("Longitude:   %d.%06d\n",
               (int)minmea_tocoord(&frame.longitude),
               (int)(minmea_tocoord(&frame.longitude) * 1e6) % 1000000);
        printk("Satellites:  %d\n", frame.satellites_tracked);
        printk("Altitude:  %d.%02d m\n",
               (int)minmea_tofloat(&frame.altitude),
               (int)(minmea_tofloat(&frame.altitude) * 100) % 100);
        printk("Fix quality:    %d\n", frame.fix_quality);
        printk("--------------------------------\n");
    }
}

K_THREAD_DEFINE(gps_tid, 2048, gps_processing_thread, NULL, NULL, NULL, 7, 0, 0);

int main(void)
{
    if (!device_is_ready(gps_dev)) {
        printk("GPS Device not ready\n");
        return 0;
    }

    uart_irq_callback_user_data_set(gps_dev, serial_cb, NULL);
    uart_irq_rx_enable(gps_dev);
    printk("GPS Device is ready\n");
    return 0;
}