# Scrolling Text with ESP32 and MAX7219

## Prerequsites

### Hardware
- MCU ESP32-WROOM-32
- LED Matrix Display MAX7219
- Real Time Clock DS3231
- Rotary Encoder with push button HW-040 
- Battery CS2032

### Software
 - PlatformIO in VS Code https://docs.platformio.org/
 - MD_MAX72XX https://github.com/MajicDesigns/MD_MAX72XX
 - Ai ESP32 RotaryEncoder https://github.com/igorantolic/ai-esp32-rotary-encoder
 - RTClib https://github.com/adafruit/RTClib

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
| GPIO16 | CLK    |
| GPIO19 | DT     |
| GPIO26 | SW     |
| VCC    | VCC    |
| GND    | GND    |

Display Mode select pin: 27


## References
- https://www.makerguides.com/esp32-and-max7219-matrix-display/
- https://github.com/MajicDesigns/MD_MAX72XXhttps://github.com/MajicDesigns/MD_MAX72XX