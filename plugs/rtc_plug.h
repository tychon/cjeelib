
#ifndef _RTC_PLUG_H
#define _RTC_PLUG_H

#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"

// This I2C address is hardwired and not configurable
#define RTC_ADDRESS 0x68

/*
// enum of days of the week for binary time format
typedef enum {
  MON, TUE, WED, THU, FRI, SAT, SUN
} rtc_day;

// enum of months of year for binary time format
typedef enum {
  JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
} rtc_month;
*/

// TODO offer strings commented out

typedef struct {
  // bcd & binary: range 0 - 56
  uint8_t seconds;
  // bcd & binary: range 0 - 56
  uint8_t minutes;
  // hour of day
  // bcd & binary: range 0 - 23
  uint8_t hours;
  // day of week
  // bcd: range 1 - 7
  // binary: range 0 - 6
  uint8_t day;
  // day of month
  // bcd: range 1 - 31
  // binary: range 0 - 30
  uint8_t date;
  // month of year
  // bcd: range 1 - 12
  // binary: range 0 - 11
  uint8_t month;
  // bcd & binary: year of century; range 0 - 99
  uint8_t year;
  // bcd & binary: new century
  // (toggled if CEB bit is set and years roll over)
  bool century;
  uint8_t calibration;
} rtc_time;

uint8_t binary_to_bcd(uint8_t binary);
uint8_t bcd_to_binary(uint8_t bcd);

// take time in bcd!
bool rtc_read_time(i2cport *port, rtc_time *time);
// take time in bcd!
bool rtc_set_time(i2cport *port, rtc_time *time);

rtc_time *rtc_convert_time_to_binary(rtc_time *time);
rtc_time *rtc_convert_time_to_bcd(rtc_time *time);

#endif // _RTC_PLUG_H

