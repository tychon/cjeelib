
/**

You can use `usart_simple_setup(BAUDRATE)` when `baudrate` is a macro defined
like `#define BAUDRATE 57600` or you set up your usart by hand:
Steps for setting up the USART:
* select a USART mode
* set up clock generation / baud rate
* set up frame format (data bits, stop bits, parity)
* turn circuitry for receiver / transmitter on
NOTE: The global interrupt flag should be disabled while setting up interrupts.

If you use usart_printf, the maximum string length is 512 characters by default.
You can change this by defining USART_STRINGFTM_BUFFERSIZE to your favorite
value. But be aware, that there is a static buffer that lingers in you stack
with this size.

 */

#ifndef _USART_H
#define _USART_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

#include "bits.h"

#ifndef USART_STRINGFTM_BUFFERSIZE
  #define USART_STRINGFTM_BUFFERSIZE 512
#endif

// used later
#define usart_clear_errors() (CLRBIT(UCSR0A, FE0), CLRBIT(UCSR0A, DOR0), CLRBIT(UCSR0A, UPE0))

///// Baud Rate /////
/// Values for the UBRRn register depending on USART baud rate
// Asynchronous normal mode (U2Xn = 0)
#define BAUD_PRESCALE_ASYNC(baudr) (((F_CPU / (baudr * 16UL))) - 1)
// Asynchronous double speed mode (U2Xn = 1)
#define BAUD_PRESCALE_ASYNC_DOUBLED(baudr) (((F_CPU / (baudr * 8UL))) - 1)
// Synchronous master mode
#define BAUD_PRESCALE_SYNC(baudr) (((F_CPU / (baudr * 2UL))) - 1)

// Load value for UBRR into registers
// eg. `usart_baud(BAUD_PRESCALE_SYNC(57600));`
#define usart_baud(ubrr) (UBRR0H = (unsigned char)(ubrr << 8), UBRR0L = (unsigned char)ubrr)

#define usart_normal_transmission_speed() (usart_clear_errors(), CLRBIT(UCSR0A, U2X0))
#define usart_double_transmission_speed() (usart_clear_errors(), SETBIT(UCSR0A, U2X0))

///// USART Mode /////
#define usart_mode_async() (CLRBIT(UCSR0C, UMSEL01), CLRBIT(UCSR0C, UMSEL00))
#define usart_mode_sync()  (CLRBIT(UCSR0C, UMSEL01), SETBIT(UCSR0C, UMSEL00))
#define usart_mode_master_spi() (SETBIT(UCSR0C, UMSEL01), SETBIT(UCSR0C, UMSEL00))

///// Frame Format /////
#define usart_character_size5() (CLRBIT(UCSR0B, UCSZ02), CLRBIT(UCSR0C, UCSZ01), CLRBIT(UCSR0C, UCSZ00))
#define usart_character_size6() (CLRBIT(UCSR0B, UCSZ02), CLRBIT(UCSR0C, UCSZ01), SETBIT(UCSR0C, UCSZ00))
#define usart_character_size7() (CLRBIT(UCSR0B, UCSZ02), SETBIT(UCSR0C, UCSZ01), CLRBIT(UCSR0C, UCSZ00))
#define usart_character_size8() (CLRBIT(UCSR0B, UCSZ02), SETBIT(UCSR0C, UCSZ01), SETBIT(UCSR0C, UCSZ00))
#define usart_character_size9() (SETBIT(UCSR0B, UCSZ02), SETBIT(UCSR0C, UCSZ01), SETBIT(UCSR0C, UCSZ00))

#define usart_parity_off()  (CLRBIT(UCSR0C, UPM01), CLRBIT(UCSR0C, UPM00))
#define usart_parity_even() (SETBIT(UCSR0C, UPM01), CLRBIT(UCSR0C, UPM00))
#define usart_parity_odd()  (SETBIT(UCSR0C, UPM01), SETBIT(UCSR0C, UPM00))

#define usart_stop_1bit()  (CLRBIT(UCSR0C, USBS0))
#define usart_stop_2bits() (SETBIT(UCSR0C, USBS0))

#define usart_multi_processor_mode_on()  (usart_clear_errors(), CLRBIT(UCSR0A, MPCM0))
#define usart_multi_processor_mode_off() (usart_clear_errors(), SETBIT(UCSR0A, MPCM0))

///// Interrupts /////
#define usart_interrupt_rx_on()  (SETBIT(UCSR0B, RXCIE0))
#define usart_interrupt_rx_off() (CLRBIT(UCSR0B, RXCIE0))
#define usart_interrupt_tx_on()  (SETBIT(UCSR0B, TXCIE0))
#define usart_interrupt_tx_off() (CLRBIT(UCSR0B, TXCIE0))
#define usart_interrupt_dre_on()  (SETBIT(UCSR0B, UDRIE0))
#define usart_interrupt_dre_off() (CLRBIT(UCSR0B, UDRIE0))

///// Other /////

// When to transmit data (tx) and receive data in one clock cycle
// Used in synchronous mode only.
// Transmit on falling edge of clock:
#define usart_clock_polarity_tx_falling() (CLRBIT(UCSR0C, UCPOL0))
// Transmit on raising edge of clock:
#define usart_clock_polarity_tx_raising() (SETBIT(UCSR0C, UCPOL0))

//TODO this does not compile with -Werror=unused-but-set-variable
#define usart_flush_receive_buffer() {\
  unsigned char usart_flush_receive_buffer_dummy; \
  while (BITSET(UCSR0A, RXC0)) usart_flush_receive_buffer_dummy = UDR0; \
}

#define usart_simple_setup(baudrate) { \
  usart_baud(BAUD_PRESCALE_SYNC(baudrate)); \
  usart_mode_sync(); \
  usart_character_size8(); \
  usart_parity_off(); \
  usart_stop_1bit(); \
  usart_receiver_enable(); \
  usart_transmitter_enable(); \
}

///// Transmitting and receiving /////

#define usart_receiver_enable()  (SETBIT(UCSR0B, RXEN0))
#define usart_receiver_disable() (CLRBIT(UCSR0B, RXEN0))
#define usart_transmitter_enable() (SETBIT(UCSR0B, TXEN0))
#define usart_transmitter_disable() (CLRBIT(UCSR0B, TXEN0))

#define usart_receive_complete()  (BITSET(UCSR0A, RXC0))
#define usart_transmit_complete() (BITSET(UCSR0A, TXC0))
#define usart_data_register_empty() (BITSET(UCSR0A, UDRE0))

#define usart_wait_for_receive_complete()      loop_until_bit_is_set(UCSR0A, RXC0)
#define usart_wait_for_transmit_complete()     loop_until_bit_is_set(UCSR0A, TXC0)
#define usart_wait_for_empty_transmit_buffer() loop_until_bit_is_set(UCSR0A, UDRE0)

#define usart_frame_error() (BITSET(UCSR0A, FE0))
#define usart_data_overrun() (BITSET(UCSR0A, DOR0))
#define usart_parity_error() (BITSET(UCSR0A, UPE0))

void usart_send_byte(uint8_t byte);
bool usart_receive_byte(uint8_t *byte, bool *frame_error, bool *usart_data_overrun, bool *parity_error);

void usart_write(const char *string, size_t len);
bool usart_printf(const char *fmt, ...);
bool usart_snprintf(char *buffer, size_t buflen, const char *fmt, ...);

#endif // _USART_H

