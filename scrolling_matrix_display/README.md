# Scrolling Text with ESP32 and MAX7219

## Prerequsites

### Hardware
- MCU ESP32-WROOM-32
- LED Matrix Display MAX7219
- Real Time Clock DS3231
- Rotary Encoder with push button HW-040 
- Battery CS2032

### Software
 - Pio in VS Code
 - Library MD_MAX72XX

## Pinning
| ESP32	 | MAX7219 Module |
|--------|----------------|
| VCC    | VCC            |
| GND    | GND            |
| GPIO18 | CLK            |
| GPIO5	 | CS             |
| GPIO23 | DIN            |

| ESP32  | DS3231 |
|--------|--------|
| GPIO4  | SQW    |
| GPIO22 | SCL    |
| GPIO21 | SDA    |
| VCC    | VCC    |
| GND    | GND    |

| ESP32  | HW-040 |
|--------|--------|
| GPIO14 | CLK    |
| GPIO27 | DT     |
| GPIO26 | SW     |
| VCC    | VCC    |
| GND    | GND    |


## References
- https://www.makerguides.com/esp32-and-max7219-matrix-display/
- https://github.com/MajicDesigns/MD_MAX72XXhttps://github.com/MajicDesigns/MD_MAX72XX