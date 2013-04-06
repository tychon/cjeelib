
/**
Remember for setting up the USART:

* set up clock generation / baud rate
* set up frame format
* turn circuitry for receiver / transmitter on

NOTE: The global interrupt flag should be disabled while setting up interrupts.

 */

#ifndef _USART_H
#define _USART_H

#include <stdint.h>
#include <avr/io.h>

///// Values for the UBRRn register depending on USART baud rate /////
// Asynchronous normal mode (U2Xn = 0)
#define BAUD_PRESCALE_ASYNC(baudr) (((F_CPU / (baudr * 16UL))) - 1)
// Asynchronous double speed mode (U2Xn = 1)
#define BAUD_PRESCALE_ASYNC_DOUBLED(baudr) (((F_CPU / (baudr * 8UL))) - 1)
// Synchronous master mode
#define BAUD_PRESCALE_SYNC(baudr) (((F_CPU / (baudr * 2UL))) - 1)

// Load value for UBRR into registers
// eg. `usart_baud(BAUD_PRESCALE_SYNC(57600));`
#define usart_baud(ubrr) (UBRR0H = (unsigned char)(ubrr << 8), UBRR0L = (unsigned char)ubrr)

///// Frame Format /////

#endif // _USART_H

