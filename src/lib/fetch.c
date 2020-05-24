#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// NOTE: ARM uses Little-Endian system
// REGISTER 13 is Program Counter
// REGISTER 14 is CPSR
// Returns the value in Big Endian form, just like in spec but can be changed to Little Endian if needed.

int fetch(int *registers, int *memory){
    int PC_value = registers[13];
    int value = 0;
    for (int i = PC_value + 3; i >= PC_value; i--)
    {
        value += memory[i] << (8 * i);
    }
    return value; 
}