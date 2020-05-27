#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void writeBin(char *filename, uint32_t *nums, int n){
  FILE *file;
  file = fopen(filename, "wb");
  fwrite(nums, sizeof(nums), 2, file);
  fclose(file);
}

uint32_t translateCommand(char instr[]){
  char *command;
  strcpy(command, instr);
  char *token = 
  return 0;
}

uint32_t assemble_DataProc(char command[], char operand[]){
  return 0;
 }

uint32_t assemble_Multiply(char command[], char operand[]){return 0;}
uint32_t assemble_SDT(char command[], char operand[]){return 0;}
uint32_t assemble_Branch(char command[], char operand[]){return 0;}
uint32_t assemble_HALT(uint32_t instr){
  return 0;
}

int main(char argc, char *argv[]) {
  printf("HELLO");
  char *instrAddress;
  uint32_t *instrs;
  uint32_t nums[] = {0xABCDEF12, 2, 3, 4};
  translateCommand("WORD1: ");
  writeBin("test.bin", nums, 4);  
  return EXIT_SUCCESS;
}
