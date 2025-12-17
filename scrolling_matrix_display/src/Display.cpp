#include <Arduino.h>
#include <RTClib.h>
#include "Display.h"
#include "NarrowFont.h"
#include "WelcomeMessage.h"

// Static member initialization
Display* Display::_instance = nullptr;
char Display::_curMessage[Display::MESG_SIZE];
char Display::_newMessage[Display::MESG_SIZE];
bool Display::_newMessageAvailable = false;
uint8_t Display::_max_devices = 0;
#define CHAR_SPACING  1 // pixels between characters
#define DISPLAY_MODE_PIN 27

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
    _mx.setFont(_font);
    
    pinMode(DISPLAY_MODE_PIN, INPUT_PULLUP);
    setMessage(WELCOME_MESSAGE);
}

void Display::iterate(DateTime now, bool set_mode)
{
    bool scrollMode = (digitalRead(DISPLAY_MODE_PIN) == LOW);

    if (scrollMode) { 
        scrollText(); 
    } 
    else {
        if (set_mode) {
            if ((millis() % 500) < 250) {
                _mx.clear();
            } else {
                char time_str[6];
                sprintf(time_str, "%02d%02d", now.hour(), now.minute());
                printText(0, _max_devices - 1, time_str);
            }
        } else {
            char time_str[6];
            sprintf(time_str, "%02d%02d", now.hour(), now.minute());
            printText(0, _max_devices - 1, time_str);
        }
    }
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

void Display::printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
// Print the text string to the LED matrix modules specified.
// Message area is padded with blank columns after printing.
{
  uint8_t   state = 0;
  uint8_t   curLen;
  uint16_t  showLen;
  uint8_t   cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  _mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do     // finite state machine to print the characters in the space available
  {
    switch(state)
    {
      case 0: // Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (*pMsg == '\0')
        {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = _mx.getChar(*pMsg++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
        // !! deliberately fall through to next state to start displaying

      case 1: // display the next part of the character
        _mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2: // initialize state for displaying empty columns
        curLen = 0;
        state++;
        // fall through

      case 3:	// display inter-character spacing or end of message padding (blank columns)
        _mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;   // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

  _mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}
