#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "initialise.h"

int num_of_addresses;

uint32_t *startRegisters(void){
    uint32_t *registers;
    do
    {
        registers = calloc(17, 4);
    } while (registers == NULL);
    return registers;
}

void freeRegisters(uint32_t *registers){
    free(registers);
}

void freeMemory(uint32_t *memory){
    free(memory);
}

void printRegisters(uint32_t *registers){
    for (int i = 0; i < 17; i++)
    {
        printf("Register[%d] contains %d\n", i, registers[i]);
    }
}


void loadFile(uint32_t *memory,char *filename){
  FILE *file;
  file = fopen(filename, "rb");
  char *word = malloc(1);
  num_of_addresses = 0;
  while (fread(word, 1, 1, file) == 1){
    //printf("%x\n",*word & 0xff);
    memory[num_of_addresses] = *word & 0xff;
    num_of_addresses++;
  }
  fclose(file);
} 

uint32_t* initializeMemory(){
  uint32_t *memory;
  do {
    memory = calloc(65536, 1);
  } while (memory == NULL);

  return memory;
}

void printMemoryHex(uint32_t *memory){
    for (int i = 0; i < num_of_addresses; i++)
    {
        if (!(i % 4)){
          printf("\n[%d]  : %X ", i, memory[i]);
        } else {
          printf("%X ", memory[i]);
        }
    }
    printf("\n");
}
  
