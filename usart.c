
#include "usart.h"

/**
 * This function blocks until the transmit buffer is empty and then
 * writes your byte to it.
 */
void usart_send_byte(uint8_t byte) {
  usart_wait_for_empty_transmit_buffer();
  UDR0 = byte;
}

/**
 * This function blocks until data is received.
 * It then checks the three error flags and returns 0 if one of them is true.
 * After that it puts UDR0 into *byte and returns 1.
 * @returns 0 if one of the error flags was set or 1 on success.
 */
int usart_receive_byte(uint8_t *byte, bool *frame_error, bool *usart_data_overrun, bool *parity_error) {
  usart_wait_for_receive_complete();
  
  if ( (*frame_error = usart_frame_error()) ) return 0;
  if ( (*usart_data_overrun = usart_data_overrun()) ) return 0;
  if ( (*parity_error = usart_parity_error()) ) return 0;
  
  *byte = UDR0;
  
  return 1;
}

