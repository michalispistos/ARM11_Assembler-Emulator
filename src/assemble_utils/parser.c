#include <inttypes.h>
#include <string.h>
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include "assemble_instr.h"

#define MAX_INSTRUCTION_LENGTH (5)

enum Commands {
	       DATA_PROC,
	       MULTIPLY,
	       SDTRANS,
	       BRANCH,
	       SPECIAL
};

void writeFile(uint32_t *contents,int num_of_instructions ,char *filename){
  FILE *output = fopen(filename,"wb");
  fwrite(contents,4,num_of_instructions,output);
  fclose(output);
}

void freeTokens(char **tokens, int *N){
  for (int i = 0; i < *N; i++){
    free(tokens[i]);
  }
  free(tokens);
}

char **tokenizer(char *line, int *N){
  // splits a given line into label, opcode, operand field;
  char **tokens = calloc(MAX_INSTRUCTION_LENGTH,(sizeof (char *)));
  char *token;
  char *rest = line;
  while ((token = strtok_r(rest," ",&rest))){
    if (*N > 0){
      char *nextToken;
      while((nextToken = strtok_r(token,",",&token))){
      tokens[*N] = calloc(strlen(nextToken) + 1,sizeof(char));
	    tokens[*N] = nextToken;
	    *N = *N + 1;
      }
    } else {
      tokens[*N] = calloc(strlen(token) + 1,sizeof(char));
      tokens[*N] = token;
      *N = *N + 1;
    }
  }
  return tokens;
}

int isLabel(char* token){
  return token[strlen(token)-1] == ':';
}

void preReadCodes(map *symbol_table){
  FILE *in = fopen("assemble_utils/opcodes.txt", "r");
  char word[30];
  int N;
  while(fgets(word, 30, in)){
    N = 0;
    word[strcspn(word,"\n")] = '\0';
    char **tokens = tokenizer(word,&N);
    //assemble_function function;
    addMap(symbol_table, tokens[1],atoi(tokens[2]),NULL);
    switch(atoi(tokens[0])){
      case 0:
        set_function(symbol_table,tokens[1],assemble_data_proc);
        break;
      case 1:
        set_function(symbol_table,tokens[1],assemble_multiply);
        break;
      case 2:
        set_function(symbol_table,tokens[1],assemble_branch);
        break;
      case 3:
        set_function(symbol_table,tokens[1],assemble_branch);
        break;
      case 4:
        set_function(symbol_table,tokens[1],assemble_special);
        break;
    }
  }
  
  fclose(in);
}