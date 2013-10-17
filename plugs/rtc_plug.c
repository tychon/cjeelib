
#include <rtc_plug.h>

#include "bits.h"
#include "rtc_plug.h"

/**
 * Convert binary int to bcd representation.
 * Since one byte is 8 bits long, the input must be in range 0 - 99 to
 * prevent overflows.
 * @param binary binary number in range 0 - 99 (inclusive)
 * @returns number in bcd representation
 */
uint8_t binary_to_bcd(uint8_t binary) {
  uint16_t scratchpad = binary;
  uint16_t carrying;
  
  for (int i = 0; i < 8; i++) {
    if ((scratchpad >> 8 & 0xf) > 4) {
      carrying = (scratchpad >> 8 & 0xf) + 3;
      scratchpad &= 0xf0ff;
      scratchpad |= carrying << 8;
    }
    if ((scratchpad >> 12) > 4) {
      carrying = (scratchpad >> 12) + 3;
      scratchpad &= 0x0fff;
      scratchpad |= carrying << 12;
    }
    scratchpad <<= 1;
  }
  return (uint8_t) (scratchpad >> 8);
}

/**
 * Convert bcd representation to binary.
 * @param bcd binary coded decimal to convert
 * @returns binary representation of input
 */
uint8_t bcd_to_binary(uint8_t bcd) {
  uint16_t scratchpad = bcd;
  scratchpad <<= 8;
  uint16_t carrying;
  
  for (int i = 0; i < 8; i++) {
    scratchpad >>= 1;
    if ((scratchpad >> 8 & 0xf) > 4) {
      carrying = (scratchpad >> 8 & 0xf) - 3;
      scratchpad &= 0xf0ff;
      scratchpad |= carrying << 8;
    }
    if ((scratchpad >> 12) > 4) {
      carrying = (scratchpad >> 12) - 3;
      scratchpad &= 0x0fff;
      scratchpad |= carrying << 12;
    }
  }
  
  return (uint8_t) scratchpad;
}

bool rtc_read_time(i2cport *port, rtc_time *time) {
  if (! i2c_register_read_init(port, RTC_ADDRESS, 0x00)) return false;
  
  time->seconds = i2c_read(port, false) & SEVENBITS;
  time->minutes = i2c_read(port, false) & SEVENBITS;
  uint8_t temp = i2c_read(port, false);
  time->hours = temp & SIXBITS;
  time->century = temp >> 6 & 0x1;
  time->day = i2c_read(port, false) & THREEBITS;
  time->date = i2c_read(port, false) & SIXBITS;
  time->month = i2c_read(port, false) & FIVEBITS;
  time->year = i2c_read(port, true);
  i2c_stop(port);
  
  return true;
}

/**
 * NOTE: Sets the CEB (Enable Century) bit in register 0x02
 * NOTE: Clears the notEOSC (disable oscillator) bit in register 0x00
 * NOTE: Turns off frequency test
 * NOTE: disables trickle charger
 * NOTE: Clears OSF (oscillator stop flag)
 */
bool rtc_set_time(i2cport *port, rtc_time *time) {
  if (! i2c_register_addr(port, RTC_ADDRESS, 0x00)) return false;
  
  #define write(val) if (! i2c_write(port, val)) return false;
  write(time->seconds | (0x1 << 7));
  write(time->minutes);
  uint8_t century = time->century ? 0x1 << 6 : 0x0;
  write(time->hours | century | 0x1 << 7);
  write(time->day);
  write(time->date);
  write(time->month);
  write(time->year);
  write(0x80 | (time->calibration & SIXBITS)); // calibration
  write(0x0); // disable trickle charger
  
  // the minutes register is cleared, when the transmission isn't restartet
  // at this point. I think it may have something to do with the register
  // pointer rolling over.
  if (! i2c_register_addr(port, RTC_ADDRESS, 0x09)) return false;
  
  write(0x0); // clear OSF
  #undef write
  
  i2c_stop(port);
  
  return true;
}

rtc_time *rtc_convert_time_to_binary(rtc_time *time) {
  time->seconds = bcd_to_binary(time->seconds);
  time->minutes = bcd_to_binary(time->minutes);
  time->hours = bcd_to_binary(time->hours);
  time->day --;
  time->date = bcd_to_binary(time->date) - 1;
  time->month = bcd_to_binary(time->month) - 1;
  time->year = bcd_to_binary(time->year);
  return time;
}

rtc_time *rtc_convert_time_to_bcd(rtc_time *time) {
  time->seconds = binary_to_bcd(time->seconds);
  time->minutes = binary_to_bcd(time->minutes);
  time->hours = binary_to_bcd(time->hours);
  time->day ++;
  time->date = binary_to_bcd(time->date + 1);
  time->month = binary_to_bcd(time->month + 1);
  time->year = binary_to_bcd(time->year);
  return time;
}


