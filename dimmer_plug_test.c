
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
  
  I2C_HOLD;
  i2c_register_write(&port, ADDRESS, 0x00, 0x00); // MODE1
  i2c_register_write(&port, ADDRESS, 0x01, 0x20); // MODE2
  i2c_register_write(&port, ADDRESS, 0x12, 0x40); // GRPPWM
  i2c_register_write(&port, ADDRESS, 0x13,    5); // GRPFREQ
  i2c_register_write(&port, ADDRESS, 0x14, 0x03); // LEDOUT0
  i2c_stop(&port);
  
  char brightness = 0xA0;
  while(true) {
    i2c_register_write(&port, ADDRESS, 0x02, ++brightness);
    i2c_stop(&port);
    _delay_ms(30);
  }
}

