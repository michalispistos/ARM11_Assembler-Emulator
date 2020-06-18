#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "map.h"
#include "assemble_instr.h"
#include "assemble_instr_utils.h"
#include "common_utils.h"

#define COND (0xE << 28)
#define PC (15)
#define MOV_ARG_SIZE (0xFF)

uint32_t assemble_data_process(map *symbols, char **tokens, int N, uint32_t code)
{
  uint32_t res;
  uint32_t I = 1;
  uint32_t S = 0;
  uint32_t rn = 0;
  uint32_t rd = 0;
  uint32_t opcode = get_code(symbols, tokens[0]);
  uint32_t operand_two = 0;
  int operand_two_index = 0;

  if (N == 4 || N == 6)
  {
    //and, eor, sub, rsb, add, orr
    rd = get_value_from_token(tokens[1]);
    rn = get_value_from_token(tokens[2]);
    operand_two_index = 3;
  }
  else if (N == 3 || N == 5)
  {
    //N == 3
    operand_two_index = 2;
    if (opcode == 13)
    {
      //mov
      rd = get_value_from_token(tokens[1]);
    }
    else
    {
      //tst,teq,cmp
      rn = get_value_from_token(tokens[1]);
      S = 1;
    }
  }

  // setting operand two, I
  if (tokens[operand_two_index][0] == 'r')
    I = 0;
  if (I || N == 3 || N == 4)
  {
    operand_two = get_value_from_token(tokens[operand_two_index]);
    operand_two = immediate(operand_two);
  }
  else
  {
    int Rm = get_value_from_token(tokens[operand_two_index]);
    if (tokens[operand_two_index + 2][0] == '#')
    {
      int shift_amount = get_value_from_token(tokens[operand_two_index + 2]);
      operand_two = shifted_register_expression(Rm, tokens[operand_two_index + 1], shift_amount);
    }
    else
    {
      int R = get_value_from_token(tokens[operand_two_index + 2]);
      operand_two = shifted_register_register(Rm, tokens[operand_two_index + 1], R);
    }
  }

  res = (COND | I << 25 | opcode << 21 | S << 20 | rn << 16 | rd << 12 | operand_two);
  return res;
}

uint32_t assemble_multiply(map *symbols, char **tokens, int N, uint32_t code)
{
  assert(N == 4 || N == 5);
  uint32_t res = COND;
  int Rd = get_value_from_token(tokens[1]);
  int Rm = get_value_from_token(tokens[2]);
  int Rs = get_value_from_token(tokens[3]);
  res |= Rm;
  res |= (9) << 4;
  res |= Rs << 8;
  res |= Rd << 16;
  if (N == 5)
  { //multiply and accumalate
    int Rn = get_value_from_token(tokens[4]);
    res |= Rn << 12;
    res |= (1 << 21);
  }
  return res;
}

uint32_t assemble_sdt(map *symbols, char **tokens, int N, uint32_t instr_address)
{
  uint32_t res = 0;
  int code = get_code(symbols, tokens[0]);
  int L = code ? 1 : 0;

  int U = 1;
  int P = 0;
  int I = 0;
  int Rn = 0;
  int offset = 0;

  //temp
  char tempexpr[strlen(tokens[2])];
  char temprd[strlen(tokens[1])];
  strcpy(tempexpr, tokens[2]);
  strcpy(temprd, tokens[1]);
  //temp
  int Rd = get_value_from_token(tokens[1]);

  switch (N)
  {
  case 3:
    P = 1;
    if (tokens[2][0] == '=')
    { // LDR WITH EXPRESSION
      int expression = get_value_from_token(tokens[2]);
      if (expression < 0)
      {
        res &= (mask(24) >> 1);
        expression *= -1;
      }

      if (expression <= MOV_ARG_SIZE)
      { // IF EXPRESSION CAN FIT IN MOV ARGUMENT
        tokens[0] = "mov";
        strcpy(tokens[2], tempexpr);
        strcpy(tokens[1], temprd);
        return assemble_data_process(symbols, tokens, N, instr_address);
      }
      else
      { // EXPRESSION IS TOO LARGE
        Rn = PC;
        add_map_node(symbols->stored_expressions, "expression", expression, NULL);
        int end = symbols->end;
        offset = (end) - (instr_address + 8);
        end += 4;
        symbols->end = end;
      }
    }
    else
    { // ldr or str pre
      check_trailing_char(tokens[2], ']');
      Rn = get_value_from_token(tokens[2]);
    }
    break;

  case 4:
    if (check_trailing_char(tokens[3], ']'))
    { //pre index
      P = 1;
      if (tokens[3][0] == '#')
      { // pre-index with xpression
        int expression = get_value_from_token(tokens[3]);
        U = (expression < 0) ? 0 + (0 * (expression *= -1)) : 1;
        //if (expression < 0){
        //  U = 0;
        //  expression *= -1;
        //}
        offset = (expression); // set offset to expresion
        Rn = get_value_from_token(tokens[2]);
        res |= Rn << 16;
      }
      else if (tokens[3][0] == 'r')
      { // optional pre index
        Rn = get_value_from_token(tokens[2]);
        int Rm = get_value_from_token(tokens[3]);
        I = 1; // I set to 1
        offset = Rm;
      }
    }
    else
    {                                    //post index
      I = (tokens[3][0] == 'r') ? 1 : 0; //shifted register with no offset case
      check_trailing_char(tokens[2], ']');
      Rn = get_value_from_token(tokens[2]);
      int expression = get_value_from_token(tokens[3]);
      offset = expression;
    }
    break;

  case 6:
    if (check_trailing_char(tokens[5], ']'))
    { //pre index
      P = 1;
      I = 1;
      Rn = get_value_from_token(tokens[2]);
      int Rm = get_value_from_token(tokens[3]);
      int shift_amount = get_value_from_token(tokens[5]);
      Rm = shifted_register_expression(Rm, tokens[4], shift_amount);
      offset = Rm;
    }
    else
    { // post index - no test cases?
      Rn = 0;
      offset = 0;
    }
    break;

  default:
    perror("INVALID OR UNSUPPORTED SINGLE DATA TRANSFER INSTRUCTION");
    exit(EXIT_FAILURE);
  }

  res = (COND | 1 << 26 | I << 25 | P << 24 | U << 23 | L << 20 | Rn << 16 | Rd << 12 | offset);
  return res;
}

uint32_t assemble_branch(map *symbols, char **tokens, int N, uint32_t instr_address)
{

  uint32_t res = 0;
  uint32_t opcode = get_code(symbols, tokens[0]);

  res |= opcode << 28;
  res |= 0xa << 24;

  //offset between current address and the label
  //the +8 is added because of the way the pipeline works counter is 8 bytes
  //ahead of the  executed instruction

  uint32_t offset = (get_code(symbols, tokens[1])) - (instr_address + 8);
  //shifted by 2 bits
  offset = offset >> 2;

  res = res | (offset & mask(24));
  //printf("%u\n",offset);

  return res;
}

uint32_t assemble_special(map *symbols, char **tokens, int N, uint32_t code)
{

  if (!strcmp(tokens[0], "andeq"))
  {
    return 0;
  }
  if (!strcmp(tokens[0], "lsl"))
  {
    char **new_instr = calloc(MAX_INSTRUCTION_LENGTH, (sizeof(char *)));
    assert(new_instr);
    for (int i = 0; i < 5; i++)
    {
      new_instr[i] = calloc(10, sizeof(char));
      assert(new_instr[i]);
    }
    strcpy(new_instr[0], "mov");
    strcpy(new_instr[1], tokens[1]);
    strcpy(new_instr[2], tokens[1]);
    strcpy(new_instr[3], tokens[0]);
    strcpy(new_instr[4], tokens[2]);
    uint32_t result = assemble_data_process(symbols, new_instr, 5, code);
    for (int i = 0; i < 5; i++)
    {
      free(new_instr[i]);
    }
    free(new_instr);
    return result;
  }
  perror("NOT VALID SPECIAL INSTRUCTION");
  exit(EXIT_FAILURE);
}