
#ifndef _LCD_PLUG_H
#define _LCD_PLUG_H

#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"

// The I2C address of MCP23008 is hardwired
#define LCD_EXPANDER_ADDRESS 0x24

// select register
#define LCD_RS 4
// Start data write
#define LCD_E  6
// Backlight
#define LCD_BL 7
// The DB[4-9] are on MCP23008 output 0 - 3
// MCP23008 output 5 is a spare I/O pin

typedef struct {
  // i2c port where lcd plug is connected to
  i2cport *port;
  // status of eight output pins of output plug
  uint8_t status;
} lcd_plug;

void lcd_init(i2cport *port, lcd_plug *lcd);
void lcd_backlight(lcd_plug *lcd, bool on);

#endif // _LCD_PLUG_H

