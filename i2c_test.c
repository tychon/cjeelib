
#include <util/delay.h>

#include "jeenode_pins.h"
#include "i2c.h"

#define ADDRESS 0x26

int fail(int code) {
  if (code & 0x01) digital_on(PORTD, JP2D);
  else digital_off(PORTD, JP2D);
  if (code & 0x02) digital_on(PORTD, JP3D);
  else digital_off(PORTD, JP3D);
  if (code & 0x04) digital_on(PORTD, JP4D);
  else digital_off(PORTD, JP4D);
  _delay_ms(1000);
  return 1;
}

int main(void) {
  pin_as_output(DDDR, JP2D);
  pin_as_output(DDDR, JP3D);
  pin_as_output(DDDR, JP4D);
  
  I2CPORT1(port);
  i2c_init(&port);
  
  _delay_ms(1000);
  
  // restart for writing
  if (! i2c_start(&port, ADDRESS, I2CWRITE)) fail(4);
  // write OLAT register address
  if (! i2c_write(&port, 0x0A)) fail(5);
  // write OLAT register content (turn all latches on)
  if (! i2c_write(&port, 0xFF)) fail(6);
  
  // write something else
  if (! i2c_write(&port, 0x00)) fail(5);
  
  i2c_stop(&port);
  
  
  while(true) ;
}

