#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <assert.h>
#include "assemble_instr.h"
#include "map.h"
#include "parser.h"

enum Commands {
	       DATA_PROC,
	       MULTIPLY,
	       SDTRANS,
	       BRANCH,
	       SPECIAL
};

void write_file(uint32_t *contents,int num_of_instructions ,char *filename){
  FILE *output = fopen(filename,"wb");
  assert(output);
  fwrite(contents,4,num_of_instructions,output);
  fclose(output);
  free(contents);
}


char **tokenizer(char *line, int *N){
  // splits a given line into label, opcode, operand field;
  char **tokens = calloc(MAX_INSTRUCTIONS,(sizeof (char *)));
  assert(tokens);
  char *rest = line;
  char* token;
  while ((token = strtok_r(rest," ",&rest))){
    if (*N > 0){
      char* nextToken;
      while((nextToken = strtok_r(token,",",&token))){
	    tokens[*N] = nextToken;
	    *N = *N + 1;
      }
    } else {
      tokens[*N] = token;
      *N = *N + 1;
    }
  }
  return tokens;
}

int is_label(char* token){
  return token[strlen(token)-1] == ':';
}

void pre_read_codes(map *symbol_table){

  add_map(symbol_table, "and", 0, assemble_data_process);
  add_map(symbol_table, "eor", 1, assemble_data_process);
  add_map(symbol_table, "sub", 2, assemble_data_process);
  add_map(symbol_table, "rsb", 3, assemble_data_process);
  add_map(symbol_table, "add", 4, assemble_data_process);
  add_map(symbol_table, "orr", 12, assemble_data_process);
  add_map(symbol_table, "mov", 13, assemble_data_process);
  add_map(symbol_table, "tst", 8, assemble_data_process);
  add_map(symbol_table, "teq", 9, assemble_data_process);
  add_map(symbol_table, "cmp", 10, assemble_data_process);

  add_map(symbol_table, "mul", 0, assemble_multiply);
  add_map(symbol_table, "mla", 1, assemble_multiply);

  add_map(symbol_table, "ldr", 1, assemble_sdt);
  add_map(symbol_table, "str", 0, assemble_sdt);

  add_map(symbol_table, "beq", 0, assemble_branch);
  add_map(symbol_table, "bne", 1, assemble_branch);
  add_map(symbol_table, "bge", 10, assemble_branch);
  add_map(symbol_table, "blt", 11, assemble_branch);
  add_map(symbol_table, "bgt", 12, assemble_branch);
  add_map(symbol_table, "ble", 13, assemble_branch);
  add_map(symbol_table, "b", 14, assemble_branch);
  add_map(symbol_table, "al", 14, assemble_branch);
  
  add_map(symbol_table, "andeq", 0, assemble_special);
  add_map(symbol_table, "lsl", 0, assemble_special);
}
