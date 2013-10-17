
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
 * It then loads the error flags into `frame_error`, `usart_data_overrun` and
 * `parity_error` and returns false if one of them is true.
 * After that it puts UDR0 into *byte and returns true.
 * @returns false if one of the error flags was set or true on success.
 */
bool usart_receive_byte(uint8_t *byte, bool *frame_error, bool *usart_data_overrun, bool *parity_error) {
  usart_wait_for_receive_complete();
  
  if ( (*frame_error = usart_frame_error()) ) return false;
  if ( (*usart_data_overrun = usart_data_overrun()) ) return false;
  if ( (*parity_error = usart_parity_error()) ) return false;
  
  *byte = UDR0;
  
  return true;
}

