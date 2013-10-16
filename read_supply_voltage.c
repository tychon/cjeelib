
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"

#define USART_BAUDRATE 57600
//#define USART_BAUDRATE 500000
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

static void serial_init(void) {
  // load upper 8 bits of the baud rate into the high byte of the UBRR register
  UBRR0H = (BAUD_PRESCALE >> 8);
  // load lower 8 bits of the baud rate into the low byte of the UBRR register
  UBRR0L = BAUD_PRESCALE;
  
  // 8data,1stopbit
  UCSR0C = (0 << UMSEL00) | (1 << UCSZ00) | (1 << UCSZ01);
  // turn on the transmission and reception circuitry
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02);
}

static void sendbyte(uint8_t b) {
  // do nothing until UDR is ready for more data to be written to it
  while ((UCSR0A & (1 << UDRE0)) == 0) {};
  // memory was cleared - write to it
  UDR0 = b;
}

int main(void) {
  serial_init();
  
  for (;;) {
    double vcc = adc_convert_vcc();
    //TODO use other printf variant
    char string[100];
    int len = snprintf(string, 100, "%f", vcc);
    for (int i = 0; i < len; i++) sendbyte(string[i]);
    sendbyte('\n');
  }
}
