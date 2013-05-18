/**
 * Runs MT19937 1000 times with seed 45784 and prints out result.
 * 
 * Link with mt19937.c
 */

#include <stdio.h>
#include "mt19937.h"

void main(void) {
  mtwister_state mts;
  mt19937_init(&mts, 45784);
  for (int i = 0; i < 1000; i++) {
    printf("%zu\n", mt19937_extract(&mts));
  }
}

