#include "Display.h"
#include <Arduino.h>

// Static member initialization
Display* Display::_instance = nullptr;
char Display::_curMessage[Display::MESG_SIZE];
char Display::_newMessage[Display::MESG_SIZE];
bool Display::_newMessageAvailable = false;
uint8_t Display::_max_devices = 0;

Display::Display(uint8_t data_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t max_devices)
    : _mx(MD_MAX72XX::FC16_HW, cs_pin, max_devices)
{
    _instance = this;
    _max_devices = max_devices;
}

void Display::setup()
{
    _mx.begin();
    _mx.setShiftDataInCallback(scrollDataSource);
    _mx.setShiftDataOutCallback(scrollDataSink);
    strcpy(_curMessage, "Hello World!");
}

void Display::iterate()
{
    scrollText();
}

void Display::setMessage(const char *message)
{
    if (strlen(message) < MESG_SIZE)
    {
        strcpy(_newMessage, message);
        _newMessageAvailable = true;
    }
}

void Display::scrollText(void)
{
    const uint8_t SCROLL_DELAY = 75;
    static uint32_t prevTime = 0;

    if (millis() - prevTime >= SCROLL_DELAY)
    {
        _mx.transform(MD_MAX72XX::TSL);
        prevTime = millis();
    }
}

void Display::scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
{
    // Data scrolled off the display
}

uint8_t Display::scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
{
    static enum { S_IDLE, S_NEXT_CHAR, S_SHOW_CHAR, S_SHOW_SPACE } state = S_IDLE;
    static char *p;
    static uint16_t curLen, showLen;
    static uint8_t cBuf[8];
    const uint8_t CHAR_SPACING = 1;
    uint8_t colData = 0;

    switch (state)
    {
    case S_IDLE:
        p = _curMessage;
        if (_newMessageAvailable)
        {
            strcpy(_curMessage, _newMessage);
            _newMessageAvailable = false;
        }
        state = S_NEXT_CHAR;
        break;

    case S_NEXT_CHAR:
        if (*p == '\0')
        {
            state = S_IDLE;
        }
        else
        {
            showLen = _instance->_mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
            curLen = 0;
            state = S_SHOW_CHAR;
        }
        break;

    case S_SHOW_CHAR:
        colData = cBuf[curLen++];
        if (curLen < showLen)
            break;

        showLen = (*p != '\0' ? CHAR_SPACING : (_max_devices * COL_SIZE) / 2);
        curLen = 0;
        state = S_SHOW_SPACE;
        // fall through

    case S_SHOW_SPACE:
        curLen++;
        if (curLen == showLen)
            state = S_NEXT_CHAR;
        break;

    default:
        state = S_IDLE;
    }

    return (colData);
}
