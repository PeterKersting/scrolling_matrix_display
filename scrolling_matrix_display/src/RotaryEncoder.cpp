#include "RotaryEncoder.h"

// Initialize static members
AiEsp32RotaryEncoder RotaryEncoder::rotaryEncoder(
    ROTARY_ENCODER_CLK_PIN, 
    ROTARY_ENCODER_DT_PIN, 
    ROTARY_ENCODER_BUTTON_PIN, 
    ROTARY_ENCODER_VCC_PIN, 
    ROTARY_ENCODER_STEPS);

RotaryEncoder::RotaryEncoder() {
}

void RotaryEncoder::rotary_onButtonClick()
{
    static unsigned long lastTimePressed = 0;
    if (millis() - lastTimePressed < 500)
    {
        return;
    }
    lastTimePressed = millis();
    Serial.print(" button pressed ");
    Serial.print(millis());
    Serial.println(" milliseconds after restart");
}

void IRAM_ATTR RotaryEncoder::readEncoderISR()
{
    RotaryEncoder::rotaryEncoder.readEncoder_ISR();
}

void RotaryEncoder::setup()
{
    // Initialize left encoder
    RotaryEncoder::rotaryEncoder.begin();
    RotaryEncoder::rotaryEncoder.setup([]() { RotaryEncoder::readEncoderISR(); });
    RotaryEncoder::rotaryEncoder.setBoundaries(0, 1000, false);
    RotaryEncoder::rotaryEncoder.setAcceleration(250);
}

void RotaryEncoder::iterate()
{
    if (rotaryEncoder.encoderChanged())
    {
        Serial.print("Value: ");
        Serial.println(rotaryEncoder.readEncoder());
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        rotary_onButtonClick();
    }
    delay(10);
}