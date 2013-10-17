/**
 * Writes with a baud rate of 57600 to usart.
 * mode: sync, character size: 8 bit, no parity, 1 stop bit
 */

#include <util/delay.h>

#define USART_BAUDRATE 57600

#include "jeenode_pins.h"
#include "usart.h"

int main(void) {
  //TODO FIXME mysterious: example breaks if this line is missing
  pin_as_output(DDDR, JP1D);
  
  usart_simple_setup(USART_BAUDRATE);
  
  int counter = 0;
  for (;;) {
    usart_printf("counter c = %d\n", counter);
    if (++ counter >= 26) {
      counter = 0;
      usart_wait_for_empty_transmit_buffer();
      UDR0 = '\n';
    }
  }
}

