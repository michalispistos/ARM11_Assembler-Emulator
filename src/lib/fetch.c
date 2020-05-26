#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// NOTE: ARM uses Little-Endian system
// REGISTER 13 is Program Counter
// REGISTER 14 is CPSR
// Interprets the 4 byte instruction stored in the location addressed by PC

int fetch(int *registers, int *memory){
    int PC_value = registers[15];
    int value = 0;
    for (int i = 3; i >= 0; i--)
    {
        value += memory[PC_value + i] << (8 * i);
    }
    return value; 
}