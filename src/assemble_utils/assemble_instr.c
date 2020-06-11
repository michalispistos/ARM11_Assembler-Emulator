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
/*
static int count_zeroes(uint32_t number){
  unsigned int num_of_zeroes = 0;
  while (!(number & 1)) {
    num_of_zeroes++;
    number >>= 1;
  }
  return num_of_zeroes;
}


static int calculate(uint32_t word){
  //uint32_t res = word;
  if (word < mask(8)) return word;
  uint32_t rotateTimes = count_zeroes(word);
  return (word >> rotateTimes) | ((rotateTimes/2) << 8);
}
*/

static uint32_t rotateLeft(uint32_t word, int times){
  return (word << times) | (word >> (31-times));
}

static int calculate_rotate_value(uint32_t word){
  for (int i = 0; i < 32; i += 2){
    if (rotateLeft(word,i) <= mask(8)) return i;
  }
  perror("too large");
  exit(EXIT_FAILURE);
}

static int calculate(uint32_t word){
  int rotateTimes = calculate_rotate_value(word);
  return ((rotateTimes / 2) << 8) | (word >> (32 - rotateTimes));
}

int get_register_address(char *registers) {
  if (registers[0] == '#' || registers[0] == '='){
    // <#expression>
    memmove(registers, registers + 1, strlen(registers));
    int res;
    if (registers[0] == '0' && registers[1] == 'x'){
      //hex
    res = strtol(registers,NULL,16);
    } else if (registers[0] == '-' && registers[1] == '0' && registers[2] == 'x'){
      memmove(registers,registers+1,strlen(registers));
      res = -1 * strtol(registers,NULL,16);
    }
    else{
      //not hex
    res = strtol(registers,NULL,10);
    }
    //printf("ROTATE VALUE: %d\n",calculate_rotate_value(res));
    //res = calculate(res);
    //printf("RES IS %x %d\n",res, immediateVal(res));
    return res;
  } else if (registers[0] == '[') {
    memmove(registers,registers+1,strlen(registers));
  }
  //register
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
    
    //Operand two = Imm value => I = 1, = register => I = 0  
    //O

    uint32_t res;
    uint32_t cond = 14;
   //all data proc cond = 1110
    uint32_t I = 1;
    uint32_t S = 0;
    uint32_t rn = 0;
    uint32_t rd = 0;
    uint32_t opcode = getCode(symbols, tokens[0]);
    uint32_t operand_two = 0;
    int operand_two_index;

    if (N == 4) {
      //add, eor, sub, rsb, add, orr
      rd = get_register_address(tokens[1]);
      rn = get_register_address(tokens[2]);
      operand_two_index = 3;
    } else {
      //N == 3
      operand_two_index = 2;
      if (opcode == 13) {
        //mov
      rd = get_register_address(tokens[1]);
      } else {
        //tst,teq,cmp
        rn = get_register_address(tokens[1]);
        S = 1;
      }
    }

    // setting operand two, I 
    if (tokens[operand_two_index][0] == 'r') I = 0;


    operand_two = get_register_address(tokens[operand_two_index]);
    operand_two = calculate(operand_two);

    /*
    if (I) {
     

    } else {
      //register
    }
    */  
    
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


uint32_t assemble_sdt(map *symbols, char **tokens, int N, uint32_t instr_address) {
  uint32_t res = 0xE40 << 20;
  char *word = tokens[0];
  // NEED TO GET CODE FROM MAP
  
  int code = getCode(symbols, word);
  res |= code << 20; // Add the symbol to the result

  //temp
  char tempexpr[15];
  char temprd[10];
  strcpy(tempexpr,tokens[2]);
  strcpy(temprd,tokens[1]);
  //temp


  int Rd = get_register_address(tokens[1]);
  res |= Rd << 12;
  if (code) res |= 1 << 20; // sets L if ldr;
  
  if (N == 3){
    res |= 1 << 24; // Set the pre flag
    // This means it is either pre or expre
    res |= 1 << 23; // Set the up flag - in most cases, we'll add
    if (tokens[2][0] == '='){
      // This means it is an expression.
      // ldr only
      int expression = get_register_address(tokens[2]); 
      if (expression < 0) {res &= (mask(24) >> 1); expression*=-1;}
      if (expression <= 0xFF){
        // return MOV Rd expression
        //uint32_t assemble_data_proc(map *symbols, char **tokens, int N, uint32_t code)
        tokens[0] = "mov";
        strcpy(tokens[2],tempexpr);
        strcpy(tokens[1],temprd);
        return assemble_data_proc(symbols, tokens, N, instr_address);
      } else {
        // expression is too large and needs to be added to end
        res |= 15 << 16; //PC 
        addMap(symbols, " ", expression, NULL);
        int end = getCode(symbols, "__end");
        res |= (end) - (instr_address + 8); // OFFSET;
        end += 4;
        set_code(symbols,"__end", end);
        return res;
        // RETURN THE RESULT
      }
    } else { // ldr or str pre
      tokens[2][strcspn(tokens[2],"]")] = '\0';
      int Rn = get_register_address(tokens[2]);
      res |= Rn << 16;

    }
  } else if (N == 4){ // pre or post
      res |= 1 << 23;
    if (tokens[3][strlen(tokens[3])-1] == ']'){ //pre index
      res |= 1 << 24; 
      printf("Token IS %s\n",tokens[3]);
      tokens[3][strcspn(tokens[3],"]")] = '\0';
      int expression = get_register_address(tokens[3]);
      if (expression < 0){
        res &= ~(1 << 23);
        expression *= -1;
      }
      res |= (expression); // set offset to expresion
      int Rn = get_register_address(tokens[2]);
      res |= Rn << 16;
    } else { //post index
      res |= 1 << 25; // I set to 1
      int expression = get_register_address(tokens[3]);
      tokens[2][strcspn(tokens[2],"]")] = '\0';
      int Rn = get_register_address(tokens[2]);
      res |= Rn << 16;
      res |= (expression); // set offset to expresion
    }

  }

  // char **stuff = NULL;
  // int Rn = 0;


  // Means that the argument 
  //uint32_t code = 12312423;
  res |= code << 20;
  return res;
}


uint32_t assemble_branch(map *symbols, char **tokens, int N, uint32_t instr_address) {
    
  uint32_t res = 0;
  uint32_t opcode = getCode(symbols,tokens[0]);

  res |= opcode << 28;
  res |= 0xa << 24;

//offset between current address and the label
//the +8 is added because of the way the pipeline works counter is 8 bytes
//ahead of the  executed instruction



uint32_t offset = (getCode(symbols,tokens[1])) - (instr_address + 8);
printf("OFFSET IS %x\n",offset);
//shifted by 2 bits
offset = offset>>2 ;
  
res = res | (offset & mask(24));
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
