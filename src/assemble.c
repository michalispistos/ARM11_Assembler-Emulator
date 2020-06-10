#include <stdlib.h>

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "assemble_utils/map.h"
#include "assemble_utils/assemble_instr.h"
#include "assemble_utils/two_pass.h"
#include "assemble_utils/parser.h"


int main(int argc, char **argv) {
  map* res = createSymbolTable(argv[1]);
  preReadCodes(res);
  printMap(res);
  int num_of_instructions = 0;
  uint32_t *contents = secondPass(argv[1],res, &num_of_instructions);
  writeFile(contents,num_of_instructions,argv[2]);
  return EXIT_SUCCESS;
}
