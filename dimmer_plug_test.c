
// Test dimmer plug on PORT 2:
// Blink LED0 with raising brightnesss

#include <util/delay.h>

#include "jeenode_pins.h"
#include "i2c.h"

#define ADDRESS 0x40

void fail() {
  digital_on(DPORT, JP4D);
  _delay_ms(1000);
  digital_off(DPORT, JP4D);
}

int main(void) {
  pin_as_output(DDDR, JP4D);
  
  I2CPORT2(port);
  i2c_init(&port);
  
  // restart for writing
  if (! i2c_restart(&port, ADDRESS, I2CWRITE)) fail();
  // write MODE1 register (turn on oscillator)
  if (! i2c_write(&port, 0x00)) fail();
  if (! i2c_write(&port, 0x00)) fail();
  
  // start for writing
  if (! i2c_restart(&port, ADDRESS, I2CWRITE)) fail();
  // write MODE2 register (group blinking)
  if (! i2c_write(&port, 0x01)) fail();
  if (! i2c_write(&port, 0x20)) fail();
  
  // restart for writing
  if (! i2c_restart(&port, ADDRESS, I2CWRITE)) fail();
  // write GRPPWM register
  if (! i2c_write(&port, 0x12)) fail();
  if (! i2c_write(&port, 0x40)) fail();
  
  // restart for writing
  if (! i2c_restart(&port, ADDRESS, I2CWRITE)) fail();
  // write GRPFREQ register
  if (! i2c_write(&port, 0x13)) fail();
  if (! i2c_write(&port, 5)) fail();
  
  // restart for writing
  if (! i2c_restart(&port, ADDRESS, I2CWRITE)) fail();
  // write LEDOUT0
  if (! i2c_write(&port, 0x14)) fail();
  if (! i2c_write(&port, 0x03)) fail();
  
  // restart for writing
  if (! i2c_restart(&port, ADDRESS, I2CWRITE)) fail();
  // write LEDOUT0
  
  i2c_stop(&port);
  
  char brightness = 0xA0;
  while(true) {
    i2c_start(&port, ADDRESS, I2CWRITE);
    i2c_write(&port, 0x02);
    i2c_write(&port, ++brightness);
    i2c_stop(&port);
    _delay_ms(30);
  }
}

