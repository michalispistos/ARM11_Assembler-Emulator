#ifndef ASSEMBLE_INSTR_UTILS_H
#define ASSEMBLE_INSTR_UTILS_H

#include <stdint.h>

int calculate(uint32_t word);
int get_value_from_token(char *registers);
uint32_t shifted_register_expression(uint32_t word, char *shift_type, int shift_amount);
uint32_t shifted_register_register(uint32_t word, char *shift_type, int Rs);
int check_trailing_char(char *token, char character);

#endif