
// Address of the plug
#define ADDRESS 0x26

/**
 * This example turns toggles all outputs every 6 seconds.
 * The plug connected to port 1. A control LED may be connected to the digital
 * pin on port 4.
 */

#include <stdbool.h>
#include <util/delay.h>

#include "jeenode_pins.h"
#include "i2c.h"

#include "plugs/output_plug.c"

int main(void) {
  pin_as_output(DDDR, JP4D);
  
  I2CPORT1(port);
  i2c_init(&port);
  OPINIT(port, ADDRESS);
  
  while (true) {
    digital_on(DPORT, JP4D);
    _delay_ms(10);
    digital_off(DPORT, JP4D);
    OPSET(port, ADDRESS, 0xFF);
    _delay_ms(5990);
    
    digital_on(DPORT, JP4D);
    _delay_ms(100);
    digital_off(DPORT, JP4D);
    OPSET(port, ADDRESS, 0x00);
    _delay_ms(5900);
  }
}

