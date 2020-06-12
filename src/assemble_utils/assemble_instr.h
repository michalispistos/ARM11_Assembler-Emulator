#ifndef ASSEMBLE_INSTR_H
#define ASSEMBLT INSTR_H
#include "map.h"
//#include <inttypes.h>

uint32_t assemble_data_process(map *symbols,char **tokens, int N,uint32_t code);
uint32_t assemble_multiply(map *symbols,char **tokens, int N,uint32_t code);
uint32_t assemble_branch(map *symbols,char **tokens, int N,uint32_t code);
uint32_t assemble_special(map *symbols,char **tokens, int N,uint32_t code);
uint32_t assemble_sdt(map *symbols, char **tokens, int N, uint32_t instr_address);
#endif