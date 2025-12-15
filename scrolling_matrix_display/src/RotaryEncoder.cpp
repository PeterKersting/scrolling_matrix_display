#include "RotaryEncoder.h"

// Initialize static members
AiEsp32RotaryEncoder RotaryEncoder::rotaryEncoder(
    ROTARY_ENCODER_CLK_PIN, 
    ROTARY_ENCODER_DT_PIN, 
    ROTARY_ENCODER_BUTTON_PIN, 
    ROTARY_ENCODER_VCC_PIN, 
    ROTARY_ENCODER_STEPS);
bool RotaryEncoder::_set_mode = false;
int16_t RotaryEncoder::_encoderValue = 0;

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
    _set_mode = !_set_mode;
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
        _encoderValue = rotaryEncoder.readEncoder();
        Serial.print("Value: ");
        Serial.println(_encoderValue);
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        rotary_onButtonClick();
    }
}

bool RotaryEncoder::isSetMode() {
    return _set_mode;
}

int16_t RotaryEncoder::getEncoderValue() {
    return _encoderValue;
}