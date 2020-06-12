#define MAX_CHAR_LENGTH (511)
#define MAX_INSTRUCTIONS (10)
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "assemble_instr.h"
#include "map.h"
#include "parser.h"

map *create_symbol_table(char *filename){
  FILE *input = fopen(filename,"r");
  if (!input) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  map *symbol_table = create_map();
  char* line = calloc(MAX_CHAR_LENGTH, sizeof(char));
  uint32_t code = 0;
  int N;
  int end = 0;
  while (fgets(line,MAX_CHAR_LENGTH,input)){
    N = 0;
    if (!strcmp(line,"\n")) break;
    line[strcspn(line,"\n")] = '\0';
    char **tokens = tokenizer(line, &N);
      if(is_label(tokens[0])){
        tokens[0][strlen(tokens[0])-1] = '\0';
        add_map(symbol_table,tokens[0],code,NULL); 
      } else {
        end +=4;
        code +=4;
      }
      free(tokens);
    }
  free(line);
  fclose(input);
  add_map(symbol_table,"__end",end,NULL);
  return symbol_table;
}


uint32_t *second_pass(char* filename, map *symbols, int *num_of_instructions){
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
    if (is_label(tokens[0])){
        continue;
    }
    assemble_function func = get_function(symbols,tokens[0]);
    uint32_t result = (func)(symbols,tokens,N,code);
    contents[code/4] = result;
    free(tokens);
    code+=4;
  }
  fclose(input);
  map *end = get_map_from_word(symbols," ");
  while (end){
    contents[code/4] = end->code;
    code+=4;
    end = end->next;
  }
  free(line);
  *num_of_instructions = code/4;
  return contents;
}