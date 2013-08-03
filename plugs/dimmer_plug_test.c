
// Test dimmer plug on PORT 2:
// Turn LEDs 0 to 15 of dimmer plug on and off in some different patterns.
// Use LED on digital output on port 4 as info.

#include <util/delay.h>

#include "random/xorshift.h"
#include "jeenode_pins.h"
#include "i2c.h"

#include "plugs/dimmer_plug.c"

#define ADDRESS 0x40

void info(void) {
  digital_on(DPORT, JP4D);
  _delay_ms(10);
  digital_off(DPORT, JP4D);
}
void fail(void) {
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
  // outputs change on ACK and not on STOP
  i2c_register_write(&port, ADDRESS, MODE2, 0x0D);
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
          i2c_register_write(&port, ADDRESS, PWM(i), brightness[i]);
        }
      }
    }
    _delay_ms(2);
  }
  i2c_stop(&port);
  info();
  
  // turn off (using auto increment feature)
  i2c_register_addr(&port, ADDRESS, AUTOINCR_PWM | PWM(0));
  for(int b = 255; b > 0; b --) {
    for(int i = 0; i < 16; i++)
      i2c_write(&port, b);
    _delay_us(100);
  }
  i2c_stop(&port);
  info();
  
  // turn on
  for (int i = 15; i >= 0; i--) {
    for (int j = 0; j <= i; j ++) {
      if (j > 0) i2c_register_write(&port, ADDRESS, PWM(j-1), 0x00);
      i2c_register_write(&port, ADDRESS, PWM(j), 0xFF);
      i2c_stop(&port);
      _delay_ms(22);
    }
  }
  info();
  
  // pseudo-randomness
  for (int i = 0; i < 255; i++) {
    i2c_register_write(&port, ADDRESS, PWM((uint8_t)(xor128() % (uint32_t)16)), (uint8_t)(xor128() % (uint32_t)16));
    _delay_ms(10);
  }
  i2c_stop(&port);
  info();
  
  // all off
  i2c_register_addr(&port, ADDRESS, AUTOINCR_PWM | PWM(0));
  for (int i = 0; i < 16; i++) i2c_write(&port, 0x00);
  i2c_stop(&port);
  info();
  
  // (twinkling)
  char twinkling[16];
  for (int i = 0; i < 16; i++) twinkling[i] = 0, brightness[i] = 0;
  i2c_register_addr(&port, ADDRESS, AUTOINCR_PWM | PWM(0));
  for (uint16_t i = 0; i < 10000; i++) {
    if (i % 100 == 0) { // choose one random twinkling star
      int pos = (uint8_t)(xor128() % (uint32_t)16);
      twinkling[pos] ^= 1;
    }
    for (int i = 0; i < 16; i++) {
      if (twinkling[i] && brightness[i] < 255) brightness[i] ++;
      else if (! twinkling[i] && brightness[i] > 0) brightness[i] --;
      i2c_write(&port, brightness[i]);
    }
    _delay_ms(1);
  }
  i2c_stop(&port);
  info();
  
  // all on
  i2c_register_addr(&port, ADDRESS, AUTOINCR_PWM | PWM(0));
  for (int i = 0; i < 16; i++) i2c_write(&port, 0xFF), brightness[i] = 255;
  i2c_stop(&port);
  _delay_ms(500);
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
  _delay_ms(100);
  info();
}

