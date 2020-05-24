#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int *startRegister(void){
    int *register_;
    do
    {
      register_ = calloc(1,4);
    } while (register_ == NULL);

    return register_;    
}

int **startRegisters(void){
    int **registers;
    do
    {
        registers = calloc(17, sizeof (int*));
    } while (registers == NULL);

    for (int i = 0; i < 17; i++)
    {
        registers[i] = startRegister();
    }   
    return registers;
}

int *startMemory(void){
    int *memory;
    do
    {
        memory = calloc(16384, 4);
    } while (memory == NULL);

    return memory;
}


void printRegisters(void){
    int **registers = startRegisters();
    for (int i = 0; i < 17; i++)
    {
        printf("Register[%d] @ location %p contains %d\n", i, registers[i], *registers[i]);
    }
}

void printMemory(void){
    int *memory = startMemory();
    for (int i = 0; i < 16384; i++)
    {
        printf("Location[%d] contains %d\n", i, memory[i]);
    }
}

int main(void){
    // printRegisters();
    printMemory();
    return 0;
}
