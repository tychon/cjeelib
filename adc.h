
#ifndef _ADC_H
#define _ADC_H

#include <avr/io.h>
#include <stdbool.h>

#include "bits.h"

///// Select a voltage reference /////
// AREF, Internal Vref turned off
#define adc_ref_aref() (CLRBIT(ADMUX, REFS1), CLRBIT(ADMUX, REFS0))
// AVcc with external capacitor at AREF pin
#define adc_ref_avcc() (CLRBIT(ADMUX, REFS1), SETBIT(ADMUX, REFS0))
// Internal 1.1V voltage reference with external capacitor at AREF pin
#define adc_ref_internal() (SETBIT(ADMUX, REFS1), SETBIT(ADMUX, REFS0))

// Write low byte of result into ADCH and high byte into ADCL
// (+ clear lowest 6 bits in ADCL
#define adc_left_adjust()  (SETBIT(ADMUX, ADLAR), ADMUX &= ~(0x3f))
// Normal representation (+ clear 6 highest bits in ADCH)
#define adc_right_adjust() (CLRBIT(ADMUX, ADLAR), ADMUX &= ~(0xfc))

///// Select channel /////
// Use this one for pin 1 to 8
#define adc_channel(pin) (ADMUX &= ~(0xF), ADMUX |= pin)
// Select 1.1V bandgap voltage reference
#define adc_channel_vbg() (ADMUX &= ~(0xF), ADMUX |= 0xE)
// Select ground reference
#define adc_channel_gnd() (ADMUX |= 0xF)

///// Enable, run, disable ADC /////
// Enable ADC
#define adc_enable() (SETBIT(ADCSRA, ADEN))
// Start a conversion, ADSC in ADCSRA will become 0 on finish (nonblocking)
#define adc_start()  (SETBIT(ADCSRA, ADSC))
// Start a conversion and wait for finish (blocking)
#define adc_run()    { adc_start(); loop_until_bit_is_clear(ADCSRA, ADSC); }
// NOTE: turning off the adc will stop running conversions
#define adc_off()    (CLRBIT(ADCSRA, ADEN))

///// Auto trigger and interrupts /////
#define adc_autotrigger_on()  (SETBIT(ADCSRA, ADATE))
#define adc_autotrigger_off() (CLRBIT(ADCSRA, ADATE))

// Activate ADC Conversion Complete Interrupt (if interrupts are enabled)
#define adc_interrupt_on()  (SETBIT(ADCSRA, ADIE))
#define adc_interrupt_off() (CLRBIT(ADCSRA, ADIE))

///// Prescaler /////
#define adc_prescale0()   (ADCSRA &= ~(0x7))
#define adc_prescale2()   (ADCSRA &= ~(0x7), ADCSRA |= 0x1)
#define adc_prescale4()   (ADCSRA &= ~(0x7), ADCSRA |= 0x2)
#define adc_prescale8()   (ADCSRA &= ~(0x7), ADCSRA |= 0x3)
#define adc_prescale16()  (ADCSRA &= ~(0x7), ADCSRA |= 0x4)
#define adc_prescale32()  (ADCSRA &= ~(0x7), ADCSRA |= 0x5)
#define adc_prescale64()  (ADCSRA &= ~(0x7), ADCSRA |= 0x6)
#define adc_prescale128() (ADCSRA |= 0x7)

#ifndef adc_prescale_best
  #define adc_prescale_best() adc_prescale128()
#endif

int adc_convert(bool left_aligned);

#ifndef BANDGAP_VOLTAGE
  #define BANDGAP_VOLTAGE 1.136
#endif

double adc_convert_vcc(void);

#endif // _ADC_H

