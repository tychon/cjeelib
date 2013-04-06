
#include <avr/io.h>
#include <util/atomic.h>

#include "adc.h"

int adc_convert(bool left_aligned) {
  adc_enable();
  adc_run();
  adc_off();
  
  // retrieve result
  int result = 0;
  if (left_aligned) {
    result = ADCH << 8;
  } else {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      result = ADCH << 8;
      result |= ADCL;
    }
  }
  
  return result;
}

double adc_convert_vcc(void) {
  adc_prescale_best();
  adc_right_adjust();
  adc_ref_avcc();
  adc_channel_vbg();
  
  int value = adc_convert(false);
  return BANDGAP_VOLTAGE / ((double)value / 1024.);
}

