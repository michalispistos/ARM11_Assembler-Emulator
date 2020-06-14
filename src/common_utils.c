#include <stdio.h>
#include "common_utils.h"

uint32_t mask(int no_of_bits) {
  return (1 << no_of_bits) - 1;
}
