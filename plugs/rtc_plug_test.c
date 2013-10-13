
#define ADDRESS 0x68

#include <util/delay.h>

#include "jeenode_pins.h"
#include "i2c.h"
#include "rtc_plug.h"

#define INFOLEDPIN JP4D

void blink_bit(bool bit) {
  digital_on(DPORT, INFOLEDPIN);
  _delay_ms(100);
  digital_off(DPORT, INFOLEDPIN);
  _delay_ms(100);
  if (bit) digital_on(DPORT, INFOLEDPIN);
  _delay_ms(800);
  digital_off(DPORT, INFOLEDPIN);
  _delay_ms(100);
}

int main() {
  pin_as_output(DDDR, JP4D);
  
  I2CPORT1(port);
  i2c_init(&port);
  
  rtc_time time;
  while (true) {
    if (! rtc_read_time(&port, ADDRESS, &time)) {
      digital_on(PORTD, JP4D);
    }
    
    // TODO output over usart
  }
}

