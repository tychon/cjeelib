
#include <util/delay.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define USART_BAUDRATE 57600

#include "usart.h"
#include "jeenode_pins.h"
#include "i2c.h"
#include "rtc_plug.h"

#define INFOLEDPIN JP4D

void blink_bit(bool bit) {
  digital_on(DPORT, INFOLEDPIN);
  _delay_ms(100);
  digital_off(DPORT, INFOLEDPIN);
  _delay_ms(100);
  if (bit) digital_on(DPORT, INFOLEDPIN);
  _delay_ms(800);
  digital_off(DPORT, INFOLEDPIN);
  _delay_ms(100);
}

char usart_output_string[500];
bool usart_write_string(const char *fmt, ...) {
  // format string
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(usart_output_string, 500, fmt, ap);
  va_end(ap);
  
  // output string
  for (int i = 0; i < len; i++) {
    usart_wait_for_empty_transmit_buffer();
    UDR0 = usart_output_string[i];
  }
  
  return len < 500;
}

int main() {
  //TODO FIXME mysterious: example breaks if this line is missing
  pin_as_output(DDDR, JP1D);
  pin_as_output(DDDR, JP4D);
  
  I2CPORT2(port);
  i2c_init(&port);
  
  i2c_stop(&port);
  
  // init usart
  usart_baud(BAUD_PRESCALE_SYNC(USART_BAUDRATE));
  
  usart_mode_sync();
  usart_character_size8();
  usart_parity_off();
  usart_stop_1bit();
  
  usart_receiver_enable();
  usart_transmitter_enable();
  
  // start output loop
  rtc_time time;
  while (true) {
    _delay_ms(300);
    
    // read data
    if (! rtc_read_time(&port, &time)) {
      digital_on(PORTD, JP4D);
      usart_write_string("Error: could not read time\n");
      continue;
    }
    
    rtc_convert_time_to_binary(&time);
    
    // output data
    if (! usart_write_string("\ntime: %u:%u:%u\n", time.hours, time.minutes, time.seconds)
     || ! usart_write_string("date: %u, %u.%u.%u", time.day + 1, time.date + 1, time.month + 1, time.year)
     || ! (time.century ? usart_write_string(" + 1 century\n") : usart_write_string(" + 0 century\n"))) {
      digital_on(PORTD, JP4D);
    }
  }
}

