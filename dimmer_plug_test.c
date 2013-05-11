
// Test dimmer plug on PORT 2:
// Turn LEDs 0 to 15 on and off in some different patterns.

#include <util/delay.h>

#include "jeenode_pins.h"
#include "i2c.h"

#include "plugs/dimmer_plug.c"

#define ADDRESS 0x40

void info() {
  digital_on(DPORT, JP4D);
  _delay_ms(10);
  digital_off(DPORT, JP4D);
}
void fail() {
  digital_on(DPORT, JP4D);
  _delay_ms(1000);
  digital_off(DPORT, JP4D);
}

int main(void) {
  pin_as_output(DDDR, JP4D);
  info();
  
  I2CPORT2(port);
  i2c_init(&port);
  
  I2C_HOLD;
  // turn on oscillator
  i2c_register_write(&port, ADDRESS, MODE1, 0x00);
  // turn on all LED drivers
  for (int i = 0; i < 4; i++)
    i2c_register_write(&port, ADDRESS, LEDOUT(i), LDR_PWM_ALL);
  i2c_stop(&port);
  
  unsigned char brightness[16];
  for (int i = 0; i < 16; i++) brightness[i] = 0;
  
  // turn on
  while(brightness[15] < 255) {
    for (int i = 0; i < 16; i++) {
      if ((i == 0) || (i > 0 && brightness[i-1] > 50)) {
        if (brightness[i] < 255) {
          brightness[i] ++;
          if (! i2c_register_write(&port, ADDRESS, PWM(i), brightness[i])) fail();
        }
      }
    }
    i2c_stop(&port);
    _delay_ms(2);
  }
  info();
  
  // turn off
  for(int b = 255; b > 0; b --) {
    for(int i = 0; i < 16; i++) {
      i2c_register_write(&port, ADDRESS, PWM(i), b);
      i2c_stop(&port);
    }
  }
  info();
  
  // turn on
  for (int i = 15; i >= 0; i--) {
    for (int j = 0; j <= i; j ++) {
      if (j > 0) i2c_register_write(&port, ADDRESS, PWM(j-1), 0x00);
      i2c_register_write(&port, ADDRESS, PWM(j), 0xFF);
      i2c_stop(&port);
      _delay_ms(20);
    }
  }
  info();
  
  // turn off
  while (brightness[15] > 0) {
    for (int i = 0; i < 16; i++) {
      if ((i == 0) || (i > 0 && brightness[i-1] < 200)) {
        if (brightness[i] > 0) {
          brightness[i] --;
          if (! i2c_register_write(&port, ADDRESS, PWM(i), brightness[i])) fail();
        }
      }
    }
    i2c_stop(&port);
  }
  info();
  
  //end
  _delay_ms(10);
  info();
}

