#include <inttypes.h>
#include "map.h"
uint32_t *second_pass(char* filename, const map *symbols, int *num_of_instructions);

map *create_symbol_table(char *filename);