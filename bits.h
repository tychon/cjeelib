
#ifndef _BITBEATER_H
#define _BITBEATER_H

#include <avr/sfr_defs.h> // special function register access

#define SETBIT(R, P) (R |=  (1 << P))
#define CLRBIT(R, P) (R &= ~(1 << P))
#define TGLBIT(R, P) (R ^=  (1 << P))

// this is with some volatile in between
#define BITSET(reg, bit) (bit_is_set(reg, bit))
#define BITCLR(reg, bit) (bit_is_clear(reg, bit))

#endif // _BITBEATER_H

