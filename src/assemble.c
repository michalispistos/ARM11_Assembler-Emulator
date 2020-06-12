#include <stdlib.h>

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "assemble_utils/map.h"
#include "assemble_utils/assemble_instr.h"
#include "assemble_utils/two_pass.h"
#include "assemble_utils/parser.h"


int main(int argc, char **argv) {
  if (argc != 3) {
    perror("incorrect number of arguments.");
    exit(EXIT_FAILURE);
  }
  map* res = create_symbol_table(argv[1]);
  pre_read_codes(res);
  int num_of_instructions = 0;
  uint32_t *contents = second_pass(argv[1],res, &num_of_instructions);
  destroy_map(res);
  write_file(contents,num_of_instructions,argv[2]);
  return EXIT_SUCCESS;
}
