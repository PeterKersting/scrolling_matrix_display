# pragma once

#include <MD_MAX72xx.h>
#include <RTClib.h>

class Display
{
public:
  Display(uint8_t data_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t max_devices);
  void setup();
  void iterate(DateTime now, bool set_mode);
  void setMessage(const char *message);

private:
  static void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col);
  static uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t);
  void scrollText(void);
  void printText(uint8_t modStart, uint8_t modEnd, char *pMsg);

  MD_MAX72XX _mx;

  static const uint8_t MESG_SIZE = 255;
  static char _curMessage[MESG_SIZE];
  static char _newMessage[MESG_SIZE];
  static bool _newMessageAvailable;
  static uint8_t _max_devices;
  bool _last_display_mode = false;

  static Display* _instance;
};

