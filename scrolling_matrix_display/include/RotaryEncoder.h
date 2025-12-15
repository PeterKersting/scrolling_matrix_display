#pragma once

#include <AiEsp32RotaryEncoder.h>
#include <Arduino.h>

#define ROTARY_ENCODER_CLK_PIN 16
#define ROTARY_ENCODER_DT_PIN 19
#define ROTARY_ENCODER_BUTTON_PIN 26
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

class RotaryEncoder {
public:
    RotaryEncoder();
    void setup();
    void iterate();

private:
    void rotary_onButtonClick();
    static void IRAM_ATTR readEncoderISR();
    
    static AiEsp32RotaryEncoder rotaryEncoder;
};