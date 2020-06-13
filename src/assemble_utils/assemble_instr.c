#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "map.h"
#include "assemble_instr.h"
#include "common_utils.h"

static int calculate_rotate_value(uint32_t word){
  for (int i = 0; i < 32; i += 2){
    if (rotate_left(word,i) <= mask(8)) return i;
  }
  perror("too large");
  exit(EXIT_FAILURE);
}

static int calculate(uint32_t word){
  int rotateTimes = calculate_rotate_value(word);
  return ((rotateTimes / 2) << 8) | (word >> (32 - rotateTimes));
}

int get_value_from_token(char *registers) {
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
      //decimal
    res = strtol(registers,NULL,10);
    }
    return res;
  } else if (registers[0] == '[') {
    // if there is a square bracket this will remove
    memmove(registers,registers+1,strlen(registers));
  }
  //register
  memmove(registers, registers + 1, strlen(registers));
  int res = strtol(registers,NULL,10);
  return res;
}

//by a constant
uint32_t shifted_register_expression(uint32_t word, char* shift_type, int shift_amount){
  int type;
  if (!strcmp("lsl",shift_type)) type = 0;
  if (!strcmp("lsr",shift_type)) type = 1;
  if (!strcmp("asr",shift_type)) type = 2;
  if (!strcmp("ror",shift_type)) type = 3;
  word |= (shift_amount << 7 | type << 5);
  return word;
}
// specified by register
uint32_t shifted_register_register(uint32_t word, char* shift_type, int Rs){
  int type;
  if (!strcmp("lsl",shift_type)) type = 0;
  if (!strcmp("lsr",shift_type)) type = 1;
  if (!strcmp("asr",shift_type)) type = 2;
  if (!strcmp("ror",shift_type)) type = 3;
  word |= (1 << 4 | Rs << 8 | type << 5);
  return word;
}

uint32_t assemble_data_process(map *symbols, char **tokens, int N, uint32_t code) {

    uint32_t res;
    uint32_t cond = 14;
    uint32_t I = 1;
    uint32_t S = 0;
    uint32_t rn = 0;
    uint32_t rd = 0;
    uint32_t opcode = get_code(symbols, tokens[0]);
    uint32_t operand_two = 0;
    int operand_two_index = 0;


    if (N == 4 || N == 6) {
      //and, eor, sub, rsb, add, orr
      rd = get_value_from_token(tokens[1]);
      rn = get_value_from_token(tokens[2]);
      operand_two_index = 3;
    } else if (N == 3 || N == 5) {
      //N == 3
      operand_two_index = 2;
      if (opcode == 13) {
        //mov
      rd = get_value_from_token(tokens[1]);
      } else {
        //tst,teq,cmp
        rn = get_value_from_token(tokens[1]);
        S = 1;
      }
    } 
      
    // setting operand two, I 
    if (tokens[operand_two_index][0] == 'r') I = 0;
    if (I || N == 3 || N == 4){
      operand_two = get_value_from_token(tokens[operand_two_index]);
      operand_two = calculate(operand_two);
    } else {
      int Rm = get_value_from_token(tokens[operand_two_index]);
      if (tokens[operand_two_index + 2][0] == '#'){
        int shift_amount = get_value_from_token(tokens[operand_two_index+2]);
        operand_two = shifted_register_expression(Rm,tokens[operand_two_index+1],shift_amount);
      } else {
        int R = get_value_from_token(tokens[operand_two_index+2]);
        operand_two = shifted_register_register(Rm,tokens[operand_two_index+1],R);
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
    int Rd = get_value_from_token(tokens[1]);
    int Rm = get_value_from_token(tokens[2]);
    int Rs = get_value_from_token(tokens[3]);
    res |= Rm;
    res |= (9) << 4;
    res |= Rs << 8;
    res |= Rd << 16;
    if (N == 5){//multiply and accumalate
      int Rn = get_value_from_token(tokens[4]);
      res |= Rn << 12;
      res |= (1 << 21);
    }
    return res;
}


uint32_t assemble_sdt(map *symbols, char **tokens, int N, uint32_t instr_address) {
  uint32_t res = 0xE40 << 20;
  char *word = tokens[0];
  
  int code = get_code(symbols, word);
  res |= code << 20; // Add the symbol to the result

  //temp
  char tempexpr[15];
  char temprd[10];
  strcpy(tempexpr,tokens[2]);
  strcpy(temprd,tokens[1]);
  //temp

  int Rd = get_value_from_token(tokens[1]);
  res |= Rd << 12;
  if (code) res |= 1 << 20; // sets L if ldr;
  res |= 1 << 23;
  if (N == 3){
    res |= 1 << 24; // Set the pre flag
    // This means it is either pre or expre
     // Set the up flag - in most cases, we'll add
    if (tokens[2][0] == '='){
      // This means it is an expression.
      // ldr only
      int expression = get_value_from_token(tokens[2]); 
      if (expression < 0) {res &= (mask(24) >> 1); expression*=-1;}
      if (expression <= 0xFF){
        // return MOV Rd expression
        //uint32_t assemble_data_process(map *symbols, char **tokens, int N, uint32_t code)
        tokens[0] = "mov";
        strcpy(tokens[2],tempexpr);
        strcpy(tokens[1],temprd);
        return assemble_data_process(symbols, tokens, N, instr_address);
      } else {
        // expression is too large and needs to be added to end
        res |= 15 << 16; //PC 
        add_map_node(symbols->stored_expressions, " ", expression, NULL);
        int end = symbols->end;
        res |= (end) - (instr_address + 8); // OFFSET;
        end += 4;
        symbols->end = end;
        return res;
        // RETURN THE RESULT
      }
    } else { // ldr or str pre
      tokens[2][strcspn(tokens[2],"]")] = '\0';
      int Rn = get_value_from_token(tokens[2]);
      res |= Rn << 16;

    }
  } else if (N == 4){ // pre or post
      res |= 1 << 23;
    if (tokens[3][strlen(tokens[3])-1] == ']'){ //pre index
    tokens[3][strcspn(tokens[3],"]")] = '\0';
    res |= 1 << 24; // Pre index flag set
      if (tokens[3][0] == '#'){ // pre-index with xpression
      //tokens[3][strcspn(tokens[3],"]")] = '\0';
      int expression = get_value_from_token(tokens[3]);
      if (expression < 0){
        res &= ~(1 << 23);
        expression *= -1;
      }
      res |= (expression); // set offset to expresion
      int Rn = get_value_from_token(tokens[2]);
      res |= Rn << 16;
      } else if (tokens[3][0] == 'r') {// optional pre index
        int Rn = get_value_from_token(tokens[2]);
        int Rm = get_value_from_token(tokens[3]);
        res |= 1 << 25; // I set to 1
        res |= Rn << 16;
        res |= Rm;
      }

    } else { //post index
      if (tokens[3][0] == 'r') res |= 1 << 25; // shifted register
      tokens[2][strcspn(tokens[2],"]")] = '\0';
      int Rn = get_value_from_token(tokens[2]);
      res |= Rn << 16;
      int expression = get_value_from_token(tokens[3]);
      res |= (expression); // set offset to expresion
    }

  } else if (N == 6){ // post/pre indexing optional with a shift value
    if (tokens[5][strlen(tokens[5])-1] == ']'){ //pre index
    res |= 1 << 24; // Pre index flag set
    int Rn = get_value_from_token(tokens[2]);
    int Rm = get_value_from_token(tokens[3]);
    res |= 1 << 25; // I set to 1
    res |= Rn << 16;
    tokens[5][strcspn(tokens[5],"]")] = '\0';
    int shift_amount = get_value_from_token(tokens[5]);
    Rm = shifted_register_expression(Rm,tokens[4],shift_amount);
    res |= Rm;

    } else { // post index - no test cases?
    }

  }

  res |= code << 20;
  return res;
}


uint32_t assemble_branch(map *symbols, char **tokens, int N, uint32_t instr_address) {
    
  uint32_t res = 0;
  uint32_t opcode = get_code(symbols,tokens[0]);

  res |= opcode << 28;
  res |= 0xa << 24;

//offset between current address and the label
//the +8 is added because of the way the pipeline works counter is 8 bytes
//ahead of the  executed instruction

uint32_t offset = (get_code(symbols,tokens[1])) - (instr_address + 8);
//shifted by 2 bits
offset = offset>>2 ;
  
res = res | (offset & mask(24));
  //printf("%u\n",offset);
 
  return res;
}

#define MAX_INSTRUCTION_LENGTH (5)

uint32_t assemble_special(map *symbols, char **tokens, int N, uint32_t code) {

  if(!strcmp(tokens[0],"andeq")){
    return 0;
  }
  if(!strcmp(tokens[0],"lsl")){
    char **new_instr = calloc(MAX_INSTRUCTION_LENGTH,(sizeof (char *)));
    assert(new_instr);
    for (int i = 0; i < 5; i++){
      new_instr[i] = calloc(10,sizeof(char));
      assert(new_instr[i]);
    }
    strcpy(new_instr[0],"mov");
    strcpy(new_instr[1],tokens[1]);
    strcpy(new_instr[2],tokens[1]);
    strcpy(new_instr[3],tokens[0]);
    strcpy(new_instr[4],tokens[2]);
    uint32_t result = assemble_data_process(symbols,new_instr,5,code);
    for (int i = 0; i < 5; i++){
      free(new_instr[i]);
    }
    free(new_instr);
    return result;
  }
  perror("NOT VALID SPECIAL INSTRUCTION");
  exit(EXIT_FAILURE);
}
