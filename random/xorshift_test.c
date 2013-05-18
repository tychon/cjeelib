/**
 * Runs XOR shift 1000 times and prints out result.
 * 
 * Link with xorshift.c
 */

#include <stdio.h>
#include "xorshift.h"

void main(void) {
  for (int i = 0; i < 1000; i++) {
    printf("%ld\n", xor128());
  }
}

