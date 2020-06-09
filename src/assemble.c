#include <stdlib.h>

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "assemble_utils/map.h"

#define MAX_CHAR_LENGTH (511)
#define MAX_INSTRUCTION_LENGTH (5)

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
    addMap(symbol_table, tokens[1],atoi(tokens[2]));
  }
  
  fclose(in);
}

map *createSymbolTable(char *filename){
  FILE *input = fopen(filename,"r");
  if (!input) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  map *symbol_table = createMap();
  char* line = calloc(MAX_CHAR_LENGTH, sizeof(char));
  uint32_t code = 0;
  int N;
  while (fgets(line,MAX_CHAR_LENGTH,input)){
    N = 0;
    line[strcspn(line,"\n")] = '\0';
    char **tokens = tokenizer(line, &N);
      if(isLabel(tokens[0])){
        tokens[0][strlen(tokens[0])-1] = '\0';
        addMap(symbol_table,tokens[0],code); 
      }
      code += 4;
      //freeTokens(tokens, &N);
    }
  fclose(input);
  return symbol_table;
}



int main(int argc, char **argv) {
  map* res = createSymbolTable(argv[1]);
  preReadCodes(res);
  int num_of_instructions = 0;
  uint32_t *contents = secondPass(argv[1],&num_of_instructions);
  writeFile(contents,num_of_instructions,"assembled");
  return EXIT_SUCCESS;
}