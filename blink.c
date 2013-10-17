
#include <util/delay.h>

#include "jeenode_pins.h"

#include "blink.h"

/**
 * Blinks one short time (100ms). Then long (800ms) if bit is set, else waits
 * long and then turns off led. This function works only for digital pins.
 * Example: pin_as_output(DDDR, JP4D); blink_bit_dpin(JP4D);
 */
void blink_bit_dpin(int pin, bool bit) {
  digital_on(DPORT, pin);
  _delay_ms(100);
  digital_off(DPORT, pin);
  _delay_ms(100);
  if (bit) digital_on(DPORT, pin);
  _delay_ms(800);
  digital_off(DPORT, pin);
  _delay_ms(100);
}

/**
 * Blink three time fast (88ms) then wait 1s with LED off.
 * Works only for digital pins.
 * Example: pin_as_output(DDDR, JP4D); blink_stop_dpin(JP4D);
 */
void blink_stop_dpin(int pin) {
  for (int i = 0; i < 3; i++) {
    digital_on(DPORT, pin);
    _delay_ms(88);
    digital_off(DPORT, pin);
    _delay_ms(88);
  }
  _delay_ms(1000);
}

