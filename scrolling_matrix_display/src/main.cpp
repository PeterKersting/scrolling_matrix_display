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

void setup(void)
{
  Serial.begin(115200);
  rotaryEncoder.setup();
  display.setup();
  rtc.setup();
}

void loop(void)
{
  display.iterate();
  rotaryEncoder.iterate();
  rtc.iterate();
}

