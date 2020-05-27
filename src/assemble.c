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

enum DataProcCodes{
  and = 0,
  eor = 1,
  sub = 2,
  rsb = 3,
  add = 4,
  orr = 12,
  mov = 13,
  tst = 8,
  teq = 9,
  cmp = 10
};

uint32_t translateCommand(char instr[]){
  char *command;
  strcpy(command, instr);
  char *token = strtok(command, " ");
  strcpy(command, token);
  token = strtok(NULL, " ");
    if(token == NULL){
      // THIS MEANS IT IS A LABEL
    } 
  return 0;
}

uint32_t getRdRnOp(char rest[], char *Rd, char *Rn, int type){
  char *operand;
  char *token = strtok(rest, ",");
  switch (type)
  {
  case 0:
    strcpy(Rd, token);
    token = strtok(NULL, ",");
    strcpy(Rn, token);
    token = strtok(NULL, ",");
    strcpy(operand, token);
    break;
  case 1:
    strcpy(Rd, token);
    token = strtok(NULL, ",");
    strcpy(operand, token);
    break;
  default:
    strcpy(Rn, token);
    token = strtok(NULL, ",");
    strcpy(operand, token);
    break;
  }
  
  operand += 2;
  switch (* operand)
  {
  case 'x':
    return (uint32_t) strtol(operand, NULL, 0); 
  default:
    return atoi (operand);
  }
}

uint32_t assemble_DataProc(char command[], char rest[]){
    char *Rd;
    char *Rn;
    uint32_t operand;
    uint32_t S = 0;
    uint32_t I = 0;
    uint32_t res = (0x38 << 26);
    if (!strcmp(command, "and")){
        operand = getRdRnOp(rest, Rd, Rn, 0);
        res |= (atoi(++Rn) << 16) | (atoi(++Rd) << 12) | operand;
    } else if (!strcmp(command, "eor")){
        operand = getRdRnOp(rest, Rd, Rn, 0);
        res |= (1 << 21) | (atoi(++Rn) << 16) | (atoi(++Rd) << 12) | operand;
    }
  return res;
 }

uint32_t assemble_Multiply(char command[], char operand[]){return 0;}
uint32_t assemble_SDT(char command[], char operand[]){return 0;}
uint32_t assemble_Branch(char command[], char operand[]){return 0;}
uint32_t assemble_HALT(uint32_t instr){
  return 0;
}

int main(char argc, char *argv[]) {
  char *myWord = "WORD1df";
  char *instrAddress;
  uint32_t *instrs;
  uint32_t nums[] = {0xABCDEF12, 2, 3, 4};
  // myWord++;
  // printf("%s", myWord);
  translateCommand("WORD1:asdfsadf");
  writeBin("test.bin", nums, 4);  
  return EXIT_SUCCESS;
}
