#ifndef ASSEMBLE_INSTR_H
#define ASSEMBLT INSTR_H

#include "map.h"

//create binary encoding for a data_process instruction
uint32_t assemble_data_process(map *symbols,char **tokens, int N,uint32_t code);

//create binary encoding for a multiply instruction
uint32_t assemble_multiply(map *symbols,char **tokens, int N,uint32_t code);

//create binary encoding for a branch instruction
uint32_t assemble_branch(map *symbols,char **tokens, int N,uint32_t code);

//create binary encoding for a special instruction
uint32_t assemble_special(map *symbols,char **tokens, int N,uint32_t code);

//create binary encoding for a single data transfer instruction
uint32_t assemble_sdt(map *symbols, char **tokens, int N, uint32_t instr_address);

#endif