#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// NOTE: ARM uses Little-Endian system
// REGISTER 13 is Program Counter
// REGISTER 14 is CPSR
// Interprets the 4 byte instruction stored in the location addressed by PC

uint32_t fetch(int *registers, int *memory){
    uint32_t PC_value = registers[13];
    uint32_t value = 0;
    for (int i = 3; i >= 0; i--)
    {
        value += memory[PC_value + i] << (8 * i);
    }
    return value; 
}