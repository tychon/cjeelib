
#include "usart.h"

#define USART_BAUDRATE 57600

int main(void) {
  usart_baud(BAUD_PRESCALE_SYNC(USART_BAUDRATE));
  
  for (;;) {
  }
}

