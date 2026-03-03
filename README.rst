# ESP32 GPS Tracker (Zephyr RTOS)

A real-time GPS NMEA parser built for the **ESP32** using **Zephyr RTOS** (v3.7.0). 


## Hardware Requirements
* **Board:** ESP32-DevKitC WROOM (`esp32_devkitc_wroom`)
* **GPS Module:** GPS GY-NEO6MV2 with antenna (https://sklep.msalamon.pl/produkt/modul-gps-gy-neo6mv2-niebieski/)


### Pinout

| GPS Module | ESP32 Pin | Note |
| :--- | :--- | :--- |
| VCC | 3.3V or 5V | Depending on your GPS module requirements |
| GND | GND | Common ground |
| TX | RX Pin | E.g., GPIO 16 (Check `app.overlay`) |
| RX | TX Pin | E.g., GPIO 17 (Check `app.overlay`) |

## Software & Dependencies
* **Zephyr RTOS:** Version 3.7.0
* **Development Environment:** VS Code with Dev Containers (Docker)
* **Libraries:** `minmea` (included in `src/`)

## How to Build and Flash

This project uses Zephyr's `west` meta-tool. Open your terminal inside the Dev Container and run:

**1. Build and flash the project:**
```bash
west build -p always -b esp32_devkitc_wroom
west flash

**3. Monitor: **
```bash
pyserial-miniterm /dev/ttyUSB0 115200
(CTRL + ] to exit)

**4. Output:**
```bash
Latitude:  51.123456
Longitude: 17.123456
Satellites:  7
Altitude:  125.50 m
Fix quality:    1
--------------------------------