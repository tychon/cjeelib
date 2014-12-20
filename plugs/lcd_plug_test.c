
#include <util/delay.h>

#include "blink.h"
#include "jeenode_pins.h"

#include "lcd_plug.h"

int main() {
  pin_as_output(DDDR, JP1D);
  blink_stop_dpin(JP1D);
  
  I2CPORT4(port);
  i2c_init(&port);
  lcd_plug lcd;
  lcd_init(&port, &lcd);
  lcd_backlight(&lcd, false);
  
  while(true) {
    lcd_backlight(&lcd, true);
    blink_bit_dpin(JP1D, lcd.status & 1 << 7);
    _delay_ms(10);
    lcd_backlight(&lcd, false);
    blink_bit_dpin(JP1D, lcd.status & 1 << 7);
    _delay_ms(1000);
  }
}

