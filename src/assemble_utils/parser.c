#include <inttypes.h>
#include <string.h>
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include "assemble_instr.h"
#include <unistd.h> 
#include <assert.h>

#define MAX_INSTRUCTION_LENGTH (10)

enum Commands {
	       DATA_PROC,
	       MULTIPLY,
	       SDTRANS,
	       BRANCH,
	       SPECIAL
};

void write_file(uint32_t *contents,int num_of_instructions ,char *filename){
  FILE *output = fopen(filename,"wb");
  fwrite(contents,4,num_of_instructions,output);
  fclose(output);
  free(contents);
}


char **tokenizer(char *line, int *N){
  // splits a given line into label, opcode, operand field;
  char **tokens = calloc(MAX_INSTRUCTION_LENGTH,(sizeof (char *)));
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
  FILE *file = fopen("/home/sarveen/Documents/C_Group_Project/arm11_42/src/opcodes.txt", "r");
  if (!file) perror("preread failed");
  char word[30];
  int N;
  while(fgets(word, 30, file)){
    N = 0;
    word[strcspn(word,"\n")] = '\0';
    char **tokens = tokenizer(word,&N);
    //assemble_function function;
    add_map(symbol_table, tokens[1],atoi(tokens[2]),NULL);
    switch(atoi(tokens[0])){
      case 0:
        set_function(symbol_table,tokens[1],assemble_data_process);
        break;
      case 1:
        set_function(symbol_table,tokens[1],assemble_multiply);
        break;
      case 2:
        set_function(symbol_table,tokens[1],assemble_sdt);
        break;
      case 3:
        set_function(symbol_table,tokens[1],assemble_branch);
        break;
      case 4:
        set_function(symbol_table,tokens[1],assemble_special);
        break;
    }
    free(tokens);
  }
  fclose(file);
}
