#include <inttypes.h>
#include "map.h"
uint32_t *secondPass(char* filename, const map *symbols, int *num_of_instructions);

map *createSymbolTable(char *filename);