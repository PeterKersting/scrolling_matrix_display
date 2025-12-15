#pragma once

#include <RTClib.h>

class RealTimeClock {
public:
    RealTimeClock();
    void setup();
    void iterate();
    DateTime getNow();

private:
    RTC_DS3231 rtc;
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
};