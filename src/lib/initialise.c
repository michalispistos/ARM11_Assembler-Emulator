#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int num_of_addresses;

uint32_t *startRegisters(void){
    uint32_t *registers;
    do
    {
        registers = calloc(15, 4);
    } while (registers == NULL);
    return registers;
}


void freeRegisters(uint32_t *registers){
    free(registers);
}

void freeMemory(uint32_t *memory){
    free(memory);
}

void printRegisters(void){
    uint32_t *registers = startRegisters();
    for (int i = 0; i < 15; i++)
    {
        printf("Register[%d] contains %d\n", i, registers[i]);
    }
    freeRegisters(registers);
}


void loadFile(uint32_t *memory,char *filename){
  FILE *file;
  file = fopen(filename, "r");
  char *word = malloc(8);
  num_of_addresses = 0;
  while (fread(word,8,1,file) == 1){
    memory[num_of_addresses] = (uint32_t) strtol(word,NULL,2);
    num_of_addresses++;
  }
  fclose(file);
} 

uint32_t* initializeMemory(){
  uint32_t *memory;
  do {
    memory = calloc(65536/8,8);
  } while (memory == NULL);

  return memory;
}

void printMemoryHex(uint32_t *memory){
    for (int i = 0; i <= num_of_addresses; i++)
    {
        printf("Location[%d] contains %x\n", i, memory[i]);
    }
}
  
