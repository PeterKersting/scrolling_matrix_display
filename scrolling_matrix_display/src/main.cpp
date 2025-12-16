#include <Arduino.h>
#include "Display.h"
#include "RotaryEncoder.h"
#include "RealTimeClock.h"

// Define the number of devices we have in the chain and the hardware interface
#define MAX_DEVICES 2

// GPIO pins
#define CLK_PIN   18 // VSPI_SCK
#define DATA_PIN  23 // VSPI_MOSI
#define CS_PIN    5  // VSPI_SS

Display display(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
RotaryEncoder rotaryEncoder;
RealTimeClock rtc;
int16_t lastEncoderValue = 0;

void setup(void)
{
  Serial.begin(115200);
  rotaryEncoder.setup();
  display.setup();
  rtc.setup();
}

void loop(void)
{
  rotaryEncoder.iterate();
  bool set_mode = rotaryEncoder.isSetMode();
  if (set_mode) {
    int16_t encoderValue = rotaryEncoder.getEncoderValue();
    if (encoderValue != lastEncoderValue) {
      rtc.adjustTime(lastEncoderValue - encoderValue);
      lastEncoderValue = encoderValue;
    }
  }
  display.iterate(rtc.getNow(), set_mode);
  rtc.iterate();
  delay(10);
}

