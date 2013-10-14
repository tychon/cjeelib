
#include <rtc_plug.h>

#include "rtc_plug.h"

/**
 * Convert binary int to bcd representation.
 * Since one byte is 8 bits long, the input must be in range 0 - 99 to
 * prevent overflows.
 * @param binary binary number in range 0 - 99 (inclusive)
 * @returns number in bcd representation
 *
static uint8_t binary_to_bcd(uint8_t binary) {
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
*/

/**
 * Convert bcd representation to binary.
 * @param bcd binary coded decimal to convert
 * @returns binary representation of input
 */
static uint8_t bcd_to_binary(uint8_t bcd) {
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
  
  time->seconds = bcd_to_binary(i2c_read(port, false) & 0x7f);
  time->minutes = bcd_to_binary(i2c_read(port, false) & 0x7f);
  uint8_t temp = i2c_read(port, false);
  time->hours = bcd_to_binary(temp & 0x3f);
  time->century = temp >> 6 & 0x1;
  time->day = (i2c_read(port, false) & 0x7) - 1;
  time->date = bcd_to_binary(i2c_read(port, false) & 0x3f) - 1;
  time->month = bcd_to_binary(i2c_read(port, false) & 0x1f) - 1;
  time->year = bcd_to_binary(i2c_read(port, true));
  
  i2c_stop(port);
  return true;
}

