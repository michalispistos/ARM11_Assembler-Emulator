#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fetch.h"

// NOTE: ARM uses Little-Endian system
// REGISTER 15 is Program Counter
// REGISTER 16 is CPSR
// Interprets the 4 byte instruction stored in the location addressed by PC

uint32_t fetch(uint32_t *registers, uint32_t *memory){
    uint32_t PC_value = registers[15];
    uint32_t value = 0;
    for (int i = 3; i >= 0; i--)
    {
        value += memory[PC_value + i] << (8 * i);
    }
    return value; 
}