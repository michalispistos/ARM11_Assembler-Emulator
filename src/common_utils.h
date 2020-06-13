#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdint.h>

// Creates a mask that will show the n LSBs
uint32_t mask(int no_of_bits);

//rotates left
uint32_t rotate_left(uint32_t word, int times);

//rotates right


#endif