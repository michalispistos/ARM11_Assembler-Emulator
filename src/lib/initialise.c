#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int *startRegisters(void){
    int *registers;
    do
    {
        registers = calloc(15, 4);
    } while (registers == NULL);
    return registers;
}

int *startMemory(void){
    int *memory;
    do
    {
        memory = calloc(65536, 1);
    } while (memory == NULL);

    return memory;
}

void freeRegisters(int *registers){
    free(registers);
}

void freeMemory(int *memory){
    free(memory);
}

void printRegisters(void){
    int *registers = startRegisters();
    for (int i = 0; i < 15; i++)
    {
        printf("Register[%d] contains %d\n", i, registers[i]);
    }
    freeRegisters(registers);
}

void printMemory(void){
    int *memory = startMemory();
    for (int i = 0; i < 16384; i++)
    {
        printf("Location[%d] contains %d\n", i, memory[i]);
    }
    freeMemory(memory);
}