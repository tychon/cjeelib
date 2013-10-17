
#define SECONDS 0    // setting this to any value other than zero doesn't work
#define MINUTES 9
#define HOURS 21
#define DAY 3        // ! monday is 0 ! / range is 0 - 6 (inclusive)
#define DATE 17 - 1  // ! first day in month is 0 !
#define MONTH 10 - 1 // ! first month in year is 0 !
#define YEAR 12
#define CENTURY false
#define CALIBRATION 0x0

#include <util/delay.h>
#include "jeenode_pins.h"
#include "i2c.h"
#include "rtc_plug.h"

int main() {
  pin_as_output(DDDR, JP4D);
  
  I2CPORT2(port);
  i2c_init(&port);
  
  rtc_time time = {.seconds = SECONDS
                 , .minutes = MINUTES
                 , .hours = HOURS
                 , .day = DAY
                 , .date = DATE
                 , .month = MONTH
                 , .year = YEAR
                 , .century = CENTURY
                 , .calibration = CALIBRATION};
  
  // convert to bcd
  rtc_convert_time_to_bcd(&time);
  
  // send time to rtc plug
  if (! rtc_set_time(&port, &time)) {
    // error, rtc didn't acknowledge
    while (true) {
      digital_on(DPORT, JP4D);
      _delay_ms(40);
      digital_off(DPORT, JP4D);
      _delay_ms(40);
    }
  } else {
    // everything ok
    digital_on(DPORT, JP4D);
    _delay_ms(1000);
    digital_off(DPORT, JP4D);
    while (true) _delay_ms(100);
  }
}
