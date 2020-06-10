#ifndef ASSEMBLE_INSTR_H
#define ASSEMBLT INSTR_H
#include "map.h"
//#include <inttypes.h>

uint32_t assemble_data_proc(map *symbols,char **tokens, int N,uint32_t code);
uint32_t assemble_multiply(map *symbols,char **tokens, int N,uint32_t code);
uint32_t assemble_branch(map *symbols,char **tokens, int N,uint32_t code);
uint32_t assemble_special(map *symbols,char **tokens, int N,uint32_t code);

#endif