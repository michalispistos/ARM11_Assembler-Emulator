#define MAX_CHAR_LENGTH (511)
#define MAX_INSTRUCTIONS (10)
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "assemble_instr.h"
#include "map.h"
#include "parser.h"

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
    if (!strcmp(line,"\n")) break;
    line[strcspn(line,"\n")] = '\0';
    char **tokens = tokenizer(line, &N);
      if(isLabel(tokens[0])){
        tokens[0][strlen(tokens[0])-1] = '\0';
        addMap(symbol_table,tokens[0],code,NULL); 
      }
      code += 4;
      //freeTokens(tokens, &N);
    }
  fclose(input);
  return symbol_table;
}


uint32_t *secondPass(char* filename, map *symbols, int *num_of_instructions){
  uint32_t* contents = calloc(MAX_INSTRUCTIONS,sizeof(uint32_t));
  FILE *input = fopen(filename,"r");
  if (!input) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  char* line = calloc(MAX_CHAR_LENGTH, sizeof(char));
  int N;
  uint32_t code = 0;
  while (fgets(line,MAX_CHAR_LENGTH,input)){
    N = 0;
    if (!strcmp(line,"\n")) break;
    line[strcspn(line,"\n")] = '\0';
    char **tokens = tokenizer(line, &N);
    if (isLabel(tokens[0])){
        continue;
    }
    assemble_function func = get_function(symbols,tokens[0]);
    uint32_t result = (func)(symbols,tokens,N,code);
    contents[code/4] = result;
    //freeTokens(tokens,&N);
    code+=4;
  }
  fclose(input);
  *num_of_instructions = code/4;
  return contents;
}