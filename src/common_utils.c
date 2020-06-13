#include <stdio.h>
#include "common_utils.h"

uint32_t mask(int no_of_bits) {
  return (1 << no_of_bits) - 1;
}

uint32_t rotate_left(uint32_t word, int times){
  return (word << times) | (word >> (31-times));
}
