
#include <avr/io.h>
#include "bits.h"

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

#define pin_as_output(sfr, bit) SETBIT(sfr, bit)
#define digital_on(sfr, bit)    SETBIT(sfr, bit)
#define digital_off(sfr, bit)   CLRBIT(sfr, bit)
#define digital_write(reg, bit, val) {\
  if (val) SETBIT(reg, bit); \
  else CLRBIT(reg, bit); \
}

#define pin_as_input(sfr, bit) CLRBIT(sfr, bit)
#define digital_read(sfr, bit) BITSET(sfr, bit)
#define pin_pull_up(sfr, bit)    SETBIT(sfr, bit)
#define pin_no_pull_up(sfr, bit) CLRBIT(sfr, bit)

