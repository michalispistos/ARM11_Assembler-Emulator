#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>
#include "map.h"
#define MAX_CHAR_LENGTH (511)

uint32_t mask(int no_of_bits) {
  return (1 << no_of_bits) - 1;
}

/*static int getImmediateVal(int operand2){
  int res = 0;
  while ()
}
*/
static uint32_t immediateVal(int operand2) {
  uint32_t result = operand2 & mask(8);
  uint32_t rotateTimes = (operand2 >> 8) * 2;
  result = (result >> rotateTimes) | ((result & mask(rotateTimes)) << (32 - rotateTimes));
  return result;
}

static int count_zeroes(uint32_t word){
  unsigned int num_of_zeroes = 32;
  if (word) num_of_zeroes--; 
  if (word & 0x0000FFFF) num_of_zeroes -= 16;
  if (word & 0x00FF00FF) num_of_zeroes -= 8;
  if (word & 0x0F0F0F0F) num_of_zeroes -= 4;
  if (word & 0x33333333) num_of_zeroes -= 2;
  if (word & 0x55555555) num_of_zeroes -= 1;
  return num_of_zeroes;
}

static int calculate(uint32_t word){
  if (word < 0xE) return word;
  uint32_t rotateTimes = count_zeroes(word);
  return (word >> rotateTimes) | ((rotateTimes/2) << 8);
}


int get_register_address(char *registers) {
  if (registers[0] == '#'){
    memmove(registers, registers + 1, strlen(registers));
    int res;
    if (registers[0] == '0' && registers[1] == 'x'){
    res = strtol(registers,NULL,16);
    } else{
    res = strtol(registers,NULL,10);
    }
    res = calculate(res);
    printf("RES IS %d %d\n",res, immediateVal(res));
    return res;
  } 
  memmove(registers, registers + 1, strlen(registers));
  int res = strtol(registers,NULL,10);
  return res;
}

//IF DATA PROC IS WRONG:
// did not specify what I is, did not specify what rd/rn is in each case, 
uint32_t assemble_data_proc(map *symbols, char **tokens, int N, uint32_t code) {
    //input of form eg [add,r0,r1,r2]
    //3 types of data proc instructions:
    //1. compute results: and, eor, sub, rsb, add, orr - <opcpde> Rd, Rn, <Operand2>
    //2. single operand assignment: mov - mov Rd. <Operand2>
    //3. do not compute results, but sets CPSR flag: tst, teq, cmp - <opcode> Rn, <Operand2>
    
    
    uint32_t res;
    uint32_t cond = 14;
   //all data proc cond = 1110
    uint32_t I = 1;
    uint32_t S = 0;
    uint32_t rn = 0;
    uint32_t rd = 0;
    uint32_t opcode = getCode(symbols, tokens[0]);
    uint32_t operand_two = 0;

    if (N == 4) {
      //add, eor, sub, rsb, add, orr
      rd = get_register_address(tokens[1]);
      rn = get_register_address(tokens[2]);
      if (tokens[3][0] == 'r') I = 0;
      operand_two = get_register_address(tokens[3]);
    } else {
      //N == 3
      if (tokens[2][0]=='r') I = 0;
      operand_two = get_register_address(tokens[2]);
      if (opcode == 13) {
        //mov
      rd = get_register_address(tokens[1]);
      } else {
        //tst,teq,cmp
        rn = get_register_address(tokens[1]);
        S = 1;
      }
    }
    res = (cond << 28 | I << 25 | opcode << 21 | S << 20 | 
          rn << 16 | rd << 12 | operand_two);
    return res;
}

uint32_t assemble_multiply(map *symbols, char **tokens, int N, uint32_t code) {
    assert(N == 4 || N == 5);
    uint32_t cond = 14;
    uint32_t res = cond << 28;
    int Rd = get_register_address(tokens[1]);
    int Rm = get_register_address(tokens[2]);
    int Rs = get_register_address(tokens[3]);
    res |= Rm;
    res |= (9) << 4;
    res |= Rs << 8;
    res |= Rd << 16;
    if (N == 5){//multiply and accumalate
      int Rn = get_register_address(tokens[4]);
      res |= Rn << 12;
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

uint32_t assemble_branch(map *symbols, char **tokens, int N, uint32_t instr_address) {
    
  uint32_t res = 0;
  uint32_t opcode;

 if(!strcmp(tokens[0],"beq")){
    opcode = 0;
 }else if(!strcmp(tokens[0],"bne")){
    opcode = 1;
 }else if(!strcmp(tokens[0],"bge")){
    opcode = 10;
 }else if(!strcmp(tokens[0],"blt")){
    opcode = 11;
 }else if(!strcmp(tokens[0],"bgt")){
    opcode = 12;
 }else if(!strcmp(tokens[0],"ble")){
    opcode = 13;
 }else{
    opcode = 14;
 }
 
 res = opcode<<28 | 10<<24;

//offset between current address and the label
//the +8 is added because of the way the pipeline works counter is 8 bytes
//ahead of the  executed instruction

uint32_t offset = getCode(symbols,tokens[1]) - (instr_address + 8);
  
//shifted by 2 bits
offset = offset>>2;
  
res = res | offset;
  
  //printf("%u\n",offset);
 
  return res;
}

uint32_t assemble_special(map *symbols, char **tokens, int N, uint32_t code) {

  uint32_t res = 0;

  if(!strcmp(tokens[0],"andeq")){
    return 0;
  }
  if(!strcmp(tokens[0],"lsl")){
    //int Rn = get_register_address(tokens[1]);
   // return Rn;
  }
  return res;
}
