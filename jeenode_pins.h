
#include <avr/io.h>

#define DDDR  DDRD
#define DPORT PORTD
#define DPIN  PIND

#define JP1D      4
#define JP2D      5
#define JP3D      6
#define JP4D      7

#define ADDR  DDRC
#define APORT PORTC
#define APIN  PINC

#define JP1A      0
#define JP2A      1
#define JP3A      2
#define JP4A      3

#define pin_as_output(sfr, bit) (sfr |= 1 << bit)
#define digital_on(sfr, bit)    (sfr |= 1 << bit)
#define digital_off(sfr, bit)   (sfr &= ~(1 << bit))

