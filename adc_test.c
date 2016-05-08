/**
 * Read analog input on port 1 and compare it to Vcc/2.
 * If it's less, turn on digital output on port 1 else on port 2.
 */

#include "jeenode_pins.h"
#include "adc.h"

int main(void) {
  DDRD |= 1 << 2 | 1 << 3;
  
  // set up adc
  adc_prescale128();
  adc_channel(0);
  adc_ref_avcc();
  //adc_left_adjust();
  
  for (;;) {
    int res = adc_convert(true);
    
    if(res < (128)) {
      PORTD |= 1 << 2;
      PORTD &= ~(1 << 3);
    } else {
      PORTD |= 1 << 3;
      PORTD &= ~(1 << 2);
    }
  }
}
