#ifndef ASSEMBLE_INSTR_UTILS_H
#define ASSEMBLE_INSTR_UTILS_H

#include <stdint.h>

//calculates the immediate value encoding from operand2
int immediate(uint32_t word);

//returns the value held in token (register number or expression value)
int get_value_from_token(char *token);

//calculates the shifted register encoding for an expression
uint32_t shifted_register_expression(uint32_t word, char *shift_type, int shift_amount);

//calculates the shifted register binary encoding for an expression
uint32_t shifted_register_register(uint32_t word, char *shift_type, int Rs);

//checks if last character of given token is the given character. 
//if so, removes that character and returns true.
int check_trailing_char(char *token, char character);

#endif