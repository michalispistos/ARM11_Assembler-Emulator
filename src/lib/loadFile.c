#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int num_of_addresses;

void readFile(uint32_t *memory,char *filename){
  FILE *file;
  file = fopen(filename, "r");
  char *word = malloc(8);
  fseek(file,0,SEEK_END);
  num_of_addresses = (ftell(file)/8)-1;
  fseek(file,0,SEEK_SET);
  int i = num_of_addresses;
  while (fread(word,8,1,file) == 1){
    memory[i] = (uint32_t) atoi(word);
    i--;
  }
  fclose(file);
} 

uint32_t* initializeMemory(char *filename){
  uint32_t *memory = calloc(65536/8,8);
  readFile(memory,filename);
  return memory;
}

void printMemory(uint32_t *memory){
    for (int i = 0; i <= num_of_addresses; i++)
    {
        printf("Location[%d] contains %u\n", i, memory[i]);
    }
}

int main(int argc, char **argv) {
  if (argc != 2){
    printf("Incorrect number of arguments");
    return EXIT_FAILURE;
  }

  uint32_t *memory = initializeMemory(argv[1]);
  printMemory(memory);
  return 0;
 }
  
