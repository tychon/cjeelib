/**
 * Mersenne Twister MT19937
 *
 * Implementation from pseudocode:
 * http://en.wikipedia.org/w/index.php?title=Mersenne_twister&oldid=555485723
 * 
 * The state array is allocated in the stack, because dynamic memory allocation
 * is often difficult in microcontrollers.
 */

#ifndef _MT19937_H
#define _MT19937_H

#include <stdint.h>

/**
 * State array of the generator.
 * The first 624 numbers are used for the actual state and the last uint32_t
 * is used as index.
 */
typedef uint32_t mtwister_state[625];

/**
 * Initialize state of twister with given seed.
 */
void mt19937_init(mtwister_state *state, uint32_t seed);

/**
 * Extract a tempered pseudorandom number based on the index-th value.
 * When the index is zero, some time is spend on generating untempered numbers.
 * The index rolls over to zero, when 625th number is going to be extracted.
 */
uint32_t mt19937_extract(mtwister_state *state);

#endif // _MT19937_H

