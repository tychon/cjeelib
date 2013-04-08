
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
  
  // start for writing
  if (! i2c_start(&port, ADDRESS, I2CWRITE)) fail(1);
  // write IODIR register address
  if (! i2c_write(&port, 0x00)) fail(2);
  if (! i2c_write(&port, 0x00)) fail(3);
  
  // restart for writing
  if (! i2c_restart(&port, ADDRESS, I2CWRITE)) fail(4);
  // write GPIO register address
  if (! i2c_write(&port, 0x09)) fail(5);
  if (! i2c_write(&port, 0xFF)) fail(6);
  
  i2c_stop(&port);
  
  digital_on(PORTD, JP4D);
  digital_on(PORTD, JP3D);
  digital_on(PORTD, JP2D);
  
  while(true) ;
}

