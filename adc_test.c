/**
 * Read analog input on port 1 and compare it to Vcc/2.
 * If it's less, turn on digital output on port 1 else on port 2.
 */

#include "jeenode_pins.h"
#include "adc.h"

int main(void) {
  pin_as_output(DDDR, JP1D);
  pin_as_output(DDDR, JP2D);
  
  digital_on(DPORT, JP1D);
  digital_on(DPORT, JP2D);
  
  // set up adc
  adc_prescale128();
  adc_ref_avcc();
  adc_channel(JP1A);
  adc_left_adjust();
  
  for (;;) {
    int res = adc_convert(true);
    
    if(res < (128)) {
      digital_on(DPORT, JP1D);
      digital_off(DPORT, JP2D);
    } else {
      digital_off(DPORT, JP1D);
      digital_on(DPORT, JP2D);
    }
  }
}
