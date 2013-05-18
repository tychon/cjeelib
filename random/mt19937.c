
#include "mt19937.h"

#define MERSENNE_MAGIC_1 0x6c078965
#define MERSENNE_MAGIC_2 0x9908b0df
#define MERSENNE_MAGIC_3 0x9d2c5680
#define MERSENNE_MAGIC_4 0xefc60000

#define sstate (*state)

void mt19937_init(mtwister_state *state, uint32_t seed) {
  sstate[0] = seed;
  for (int i = 1; i < 624; i++) {
    sstate[i] = MERSENNE_MAGIC_1 * (sstate[i-1] ^ (sstate[i-1] >> 30)) + i;
  }
  sstate[624] = 0;
}

/**
 * Generate an array of 624 untempered numbers.
 */
static void generate_numbers(mtwister_state *state) {
  for (int i = 0; i < 624; i++) {
    uint32_t y = (sstate[i] & 0x80000000)       // 32nd bit of state[i]
            + (sstate[(i+1)%624] & 0x7fffffff); // first 31 bits of state[i+1]
    sstate[i] = sstate[(i+397)%624] ^ (y >> 1);
    if (y % 2 != 0) sstate[i] ^= MERSENNE_MAGIC_2;
  }
}

uint32_t mt19937_extract(mtwister_state *state) {
  int index = sstate[624];
  if (index == 0) generate_numbers(state);
  
  uint32_t y = sstate[index];
  y ^= y >> 11;
  y ^= (y << 7) & MERSENNE_MAGIC_3;
  y ^= (y << 15) & MERSENNE_MAGIC_4;
  y ^= y >> 18;
  
  sstate[624] = (sstate[624] + 1) % 624;
  return y;
}

