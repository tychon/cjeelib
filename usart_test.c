#include <util/delay.h>

#define USART_BAUDRATE 57600

#include "jeenode_pins.h"
#include "usart.h"

int main(void) {
  //TODO FIXME mysterious: example breaks if this line is missing
  pin_as_output(DDDR, JP1D);
  
  usart_baud(BAUD_PRESCALE_SYNC(USART_BAUDRATE));
  
  usart_mode_sync();
  usart_character_size8();
  usart_parity_off();
  usart_stop_1bit();
  
  usart_receiver_enable();
  usart_transmitter_enable();
  
  int counter = 0;
  for (;;) {
    usart_wait_for_empty_transmit_buffer();
    UDR0 = 'A' + counter;
    if (++ counter >= 26) {
      counter = 0;
      usart_wait_for_empty_transmit_buffer();
      UDR0 = '\n';
    }
  }
}

