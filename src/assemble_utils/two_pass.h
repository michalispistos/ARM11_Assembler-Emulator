#ifndef TWO_PASS_H
#define TWO_PASS_H

//initialises a map and stores labels with their addresses inside
map *create_symbol_table(char *filename);

//assembles file line by line and returns an array of binary instructions
uint32_t *second_pass(char *filename, map *symbols, int *num_of_instructions);


#endif