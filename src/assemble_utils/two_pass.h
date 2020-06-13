#ifndef TWO_PASS_H
#define TWO_PASS_H

uint32_t *second_pass(char* filename, map *symbols, int *num_of_instructions);
map *create_symbol_table(char *filename);

#endif