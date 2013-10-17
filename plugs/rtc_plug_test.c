/**
 * Reads time from RTC Plug on port 2 and writes it to formatted to USART
 * every 300 ms.
 */

#include <util/delay.h>
#include <stdbool.h>

#define USART_BAUDRATE 57600

#include "jeenode_pins.h"
#include "usart.h"
#include "i2c.h"

#include "rtc_plug.h"

int main() {
  //TODO FIXME mysterious: usart breaks if this line is missing
  pin_as_output(DDDR, JP1D);
  pin_as_output(DDDR, JP4D);
  
  I2CPORT2(port);
  i2c_init(&port);
  
  i2c_stop(&port);
  
  // init usart
  usart_simple_setup(USART_BAUDRATE);
  
  // start output loop
  rtc_time time;
  while (true) {
    _delay_ms(300);
    
    // read data
    if (! rtc_read_time(&port, &time)) {
      digital_on(PORTD, JP4D);
      usart_write("Error: could not read time\n", 0);
      continue;
    }
    
    rtc_convert_time_to_binary(&time);
    
    // output data
    if (! usart_printf("\ntime: %u:%u:%u\n", time.hours, time.minutes, time.seconds)
     || ! usart_printf("date: %u, %u.%u.%u", time.day + 1, time.date + 1, time.month + 1, time.year)
     || ! (time.century ? usart_write(" + 1 century\n", 0), 1 : usart_write(" + 0 century\n", 0), 1)) {
      digital_on(PORTD, JP4D);
    }
  }
}

