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
  printf("Registers:\n");
    for (int i = 0; i < 10; i++)
    {
      printf("$%d  :",i);
      printf(" %10u (0x%08x)\n",registers[i],registers[i]);
    }
    for (int i = 10; i < 13; i++){
      printf("$%d :",i);
      printf(" %10u (0x%08x)\n",registers[i],registers[i]);
    }
    printf("PC  : %10u (0x%08x)\n",registers[15],registers[15]);
    printf("CPSR: %10d (0x%08x)\n",registers[16],registers[16]);
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
  printf("Non-zero memory:");
    for (int i = 0; i < num_of_addresses; i++)
    {
        if (!(i % 4)){
          printf("\n0x%08x: 0x%02x", i, memory[i]);
        } else {
          printf("%02x", memory[i]);
        }
    }
    printf("\n");
    
}
  
