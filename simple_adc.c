/**
 * Read analog input on port 1 and compare it to Vcc/2.
 * If it's less, turn on digital output on port 1 else on port 2.
 */

#include "jeenode_pins.h"
#include "adc.h"

int main(void) {
  /*
  // Code using using plain registers for getting diodes blinking
  DDDR = (1 << JP1D) | (1 << JP2D); // set port 1 & 2 digital as output
  
  DPORT |= (1 << JP1D); // Turn on LED1 
  DPORT |= (1 << JP2D); // Turn on LED2 
  
  // set up adc
  adc_prescale128();
  adc_ref_avcc();
  adc_channel(0);
  adc_left_adjust();
  
  for (;;) {
    int res = adc_convert(true);
    
    if(res < (128)) { 
      DPORT |= (1 << JP1D); // Turn on LED1 
      DPORT &= ~(1 << JP2D); // Turn off LED2 
    } else {
      DPORT &= ~(1 << JP1D); // Turn off LED1 
      DPORT |= (1 << JP2D); // Turn on LED2 
    }
  } */
  
  // Code using macros for output registers
  pin_as_output(DDDR, JP1D);
  pin_as_output(DDDR, JP2D);
  
  digital_on(DPORT, JP1D);
  digital_on(DPORT, JP2D);
  
  // set up adc
  adc_prescale128();
  adc_ref_avcc();
  adc_channel(0);
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
