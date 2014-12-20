
#include "output_plug.c"

#include "lcd_plug.h"

void lcd_init(i2cport *port, lcd_plug *lcd) {
  lcd->port = port;
  lcd->status = 0x0;
  OPINIT(*port, LCD_EXPANDER_ADDRESS);
}

void lcd_backlight(lcd_plug *lcd, bool on) {
  if (on) lcd->status |= 1 << LCD_BL;
  else    lcd->status &= ~(1 << LCD_BL);
  OPSET(* lcd->port, LCD_EXPANDER_ADDRESS, lcd->status);
}

