
#ifndef _RTC_PLUG_H
#define _RTC_PLUG_H

#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"

// This I2C address is hardwired and not configurable
#define RTC_ADDRESS 0x68

typedef enum {
  MON, TUE, WED, THU, FRI, SAT, SUN
} rtc_day;

typedef enum {
  JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
} rtc_month;

typedef struct {
  uint8_t seconds; // range 0 - 56
  uint8_t minutes; // range 0 - 56
  uint8_t hours;   // hour of day; range 0 - 23
  rtc_day day;     // day of week; range 0 - 6
  uint8_t date;    // day of month; range 0 - 30
  rtc_month month; // month of year; range 0 - 11
  uint8_t year;    // year of century; range 0 - 99
  bool    century; // new century (toggled if CEB bit is set and years roll over)
} rtc_time;

bool rtc_read_time(i2cport *port, rtc_time *time);
bool rtc_set_time(i2cport *port, rtc_time *time);

#endif // _RTC_PLUG_H

