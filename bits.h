
#ifndef _BITBEATER_H
#define _BITBEATER_H

#include <avr/sfr_defs.h> // special function register access

#define SETBIT(R, P) (R |=  (1 << P))
#define CLRBIT(R, P) (R &= ~(1 << P))
#define TGLBIT(R, P) (R ^=  (1 << P))

// this is with some volatile in between
#define BITSET(reg, bit) (bit_is_set(reg, bit))
#define BITCLR(reg, bit) (bit_is_clear(reg, bit))

#define TWOBITS   0x03 //      0b11
#define THREEBITS 0x07 //     0b111
#define FOURBITS  0x0f //    0b1111
#define FIVEBITS  0x1f //   0b11111
#define SIXBITS   0x3f //  0b111111
#define SEVENBITS 0x7f // 0b1111111

#endif // _BITBEATER_H

