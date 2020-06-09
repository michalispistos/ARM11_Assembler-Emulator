#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "map.h"

#define MAX_CHAR_LENGTH (511)

void secondPass(char* filename){
  FILE *input = fopen(filename,"r");
  if (!input) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  char* line = calloc(MAX_CHAR_LENGTH, sizeof(char));
  int N;
  uint32_t code;
  while (fgets(line,MAX_CHAR_LENGTH,input)){
    N = 0;
    line[strcspn(line,"\n")] = '\0';
    char **tokens = tokenizer(line, &N);
    /*
    void *function = get_funcmap(tokens[0],code);
    (*function) (tokens[0])
    freeTokens(tokens);
    }
    */
    code +=4;
  fclose(input);
}

int get_register_address(char *registers) {
  memmove(registers, registers + 1, strlen(registers));
  int res = atoi(registers);
  return res;
}

//IF DATA PROC IS WRONG:
// did not specify what I is, did not specify what rd/rn is in each case, 
uint32_t assemble_data_proc(const char **instr, int N, const map *symbols) {
    //input of form eg [add,r0,r1,r2]
    //3 types of data proc instructions:
    //1. compute results: and, eor, sub, rsb, add, orr - <opcpde> Rd, Rn, <Operand2>
    //2. single operand assignment: mov - mov Rd. <Operand2>
    //3. do not compute results, but sets CPSR flag: tst, teq, cmp - <opcode> Rn, <Operand2>
    uint32_t res;
    uint32_t cond = 14;
   //all data proc cond = 1110
    uint32_t I = 0;
    uint32_t S = 0;
    uint32_t rn = 0;
    uint32_t rd = 0;
    uint32_t opcode = getCode(symbols, instr[0]);
    uint32_t operand_two = 0;

    if (N == 4) {
      //add, eor, sub, rsb, add, orr
      rd = get_register_address(instr[1]);
      rn = get_register_address(instr[2]);
      operand_two = get_register_address(instr[3]);
    } else {
      //N == 3
      operand_two = get_register_address(instr[2]);
      if (opcode == 13) {
        //mov
      rd = get_register_address(instr[1]);
      } else {
        //tst,teq,cmp
        rn = get_regsiter_address(instr[1]);
        S = 1;
      }
    }
    res = (cond << 28 || I << 25 || opcode << 21 || S << 20 || 
          rn << 16 || rd << 12 || operand_two);
    return res;
}

uint32_t assemble_multiply(char** tokens,int N, const map *symbols) {
    assert(N == 4 || N == 5);
    uint32_t cond = 14;
    uint32_t res = cond << 28;
    int Rd = get_register_address(tokens[1]);
    int Rm = get_register_address(tokens[2]);
    int Rn = get_register_address(tokens[3]);
    res |= Rm;
    res |= (9) << 4;
    res |= Rn << 12;
    res |= Rd << 16;
    if (N == 5){//multiply and accumalate
      int Rs = tokens[4];
      res |= Rs << 8;
      res |= (1 << 21);
    }
    return res;
}
/*
uint32_t assemble_sdt(const char **tokens, int N, const map *symbols) {
  uint32_t res = 0xE40 << 19;
  char *word = tokens[0];
  // NEED TO GET CODE FROM MAP
  uint32_t code = 12312423;
  res |= code << 20;
  return res;
}
*/

uint32_t assemble_branch(const char** tokens,const map* symbols,int instr_address) {
    
  uint32_t res = 0;
  uint32_t opcode;

 if(!strcmp(tokens[0],"beq")){
    opcode = 0;
 }else if(!strcmp(tokens[0],"bne")){
    opcode = 1;
 }else if(!strcmp(tokens[0],"bge")){
    opcode = 1010;
 }else if(!strcmp(tokens[0],"blt")){
    opcode = 1011;
 }else if(!strcmp(tokens[0],"bgt")){
    opcode = 1100;
 }else if(!strcmp(tokens[0],"ble")){
    opcode = 1101;
 }else{
    opcode = 1110;
 }
 
 res = opcode<<28 | 1010<<24;

//offset between current address and the label
 int offset = instr_address - getCode(tokens[1]);
  
//shifted by 2 bits
offset = offset>>2;
  
res = res | offset;
  
  return res;
}

uint32_t assemble_special(const char** tokens,const map* symbols) {

  uint32_t res;

  if(strcmp(tokens[0],"andeq")==0){
    return 0;
  }
  if(strcmp(tokens[0],"lsl")==0){
    int Rn = get_register_address(tokens[1]);
  }


}