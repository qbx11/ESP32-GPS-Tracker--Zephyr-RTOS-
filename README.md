# ESP32 GPS Tracker (Zephyr RTOS)

A real-time GPS NMEA parser built for the **ESP32** using **Zephyr RTOS** (v3.7.0). 

This project is the first component of a larger GPS-based orienteering tracking system.
Instead of traditional checkpoint chips used in orienteering races, each runner carries a dedicated GPS device that continuously records their position during the event. After finishing the race, the device will transmit recorded data via Wi-Fi to a central administrator system.

The full system (currently under development as part of Software Development Platforms course) will provide:
- Automatic route verification
- Detection of missed checkpoints
- Centralized race data storage
- Post-race visualization of the runner’s route on a map with performance statistics

  
---

## Hardware Requirements

| Component | Details |
| :--- | :--- |
| Board | ESP32-DevKitC WROOM (`esp32_devkitc_wroom`) |
| GPS Module | GPS GY-NEO6MV2 with antenna ([sklep.msalamon.pl](https://sklep.msalamon.pl/produkt/modul-gps-gy-neo6mv2-niebieski/)) |

### Pinout

| GPS Module | ESP32 Pin  
| :---       | :---       
| VCC        | 3.3V or 5V 
| GND        | GND        
| TX         | RX Pin     
| RX         | TX Pin     

---

## Software & Dependencies

| | Version / Details |
| :--- | :--- |
| Zephyr RTOS | 3.7.0 |
| Development Environment | VS Code with Dev Containers (Docker) |
| Libraries | `minmea` (included in `src/`) |

---

## Build and Flash

This project uses Zephyr's `west`.

**1. Build the project:**
```
west build -p always -b esp32_devkitc_wroom
```

**2. Flash the project:**
```
west flash
```

**3. Monitor serial output:**
```
pyserial-miniterm /dev/ttyUSB0 115200
```
> Press `CTRL + ]` to exit.

---

## Expected Output

```
Latitude:    51.123456
Longitude:   17.123456
Satellites:  7
Altitude:    125.50 m
Fix quality: 1
--------------------------------
```
