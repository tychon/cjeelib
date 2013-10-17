
#include <stdarg.h>
#include <stdio.h>

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

/**
 * Write string to usart. If len is 0 write until first NULL byte in string,
 * else write len bytes from string.
 * Blocks until the last character was written to UDR0. After calling this
 * function use usart_wait_for_empty_transmit_buffer to wait until the
 * last bit was sent and UDR0 is free again.
 */
void usart_write(const char *string, size_t len) {
  if (len) {
    for (int i = 0; i < len; i++) {
      usart_wait_for_empty_transmit_buffer();
      UDR0 = string[i];
    }
  } else {
    do {
     usart_wait_for_empty_transmit_buffer();
     UDR0 = *string;
    } while ( *(++string) );
  }
}

static char usart_output_string[USART_STRINGFTM_BUFFERSIZE];
/**
 * Format a string with 'vsnprintf' from stdio.h and output it to the USART.
 * Blocks until the last character was written to UDR0. After calling this
 * function use usart_wait_for_empty_transmit_buffer to wait until the
 * last bit was sent and UDR0 is free again.
 * 
 * NOTE: The this function is not interrupt save, since all calls use the same
 * static buffer. Use usart_snprintf for an interrupt save variant.
 * 
 * The maximum string length is 512 characters by default. You can change this
 * by defining USART_STRINGFTM_BUFFERSIZE to your favorite value. But be aware,
 * that there is a static buffer with this size lingering in you stack.
 * 
 * @returns true, if the whole string was formatted correctly. When is was too
 * long, false is returned.
 */
bool usart_printf(const char *fmt, ...) {
  // format string
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(usart_output_string, USART_STRINGFTM_BUFFERSIZE, fmt, ap);
  va_end(ap);
  
  // output string
  for (int i = 0; i < len; i++) {
    usart_wait_for_empty_transmit_buffer();
    UDR0 = usart_output_string[i];
  }
  
  return len < USART_STRINGFTM_BUFFERSIZE;
}

/**
 * Works like usart_printf with your own buffer.
 * The formatted string is saved in buffer and send over USART.
 */
bool usart_snprintf(char *buffer, size_t buflen, const char *fmt, ...) {
  // format string
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(buffer, buflen, fmt, ap);
  va_end(ap);
  
  // output string
  for (int i = 0; i < len; i++) {
    usart_wait_for_empty_transmit_buffer();
    UDR0 = buffer[i];
  }
  
  return len < buflen;
}

