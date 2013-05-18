/**
 * Simple XOR shift pseudo random number generator.
 * Fixed seed :-)
 * 
 * Implementation from:
 * http://en.wikipedia.org/w/index.php?title=Xorshift&oldid=543383848
 */

#ifndef _XORSHIFT_H
#define _XORSHIFT_H

#include <stdint.h>

uint32_t xor128(void);

#endif // _XORSHIFT_H
