#ifndef DISPLAY_H
#define DISPLAY_H

#include <MD_MAX72xx.h>

class Display
{
public:
  Display(uint8_t data_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t max_devices);
  void setup();
  void iterate();
  void setMessage(const char *message);

private:
  static void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col);
  static uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t);
  void scrollText(void);

  MD_MAX72XX _mx;

  static const uint8_t MESG_SIZE = 255;
  static char _curMessage[MESG_SIZE];
  static char _newMessage[MESG_SIZE];
  static bool _newMessageAvailable;
  static uint8_t _max_devices;

  static Display* _instance;
};

#endif // DISPLAY_H
