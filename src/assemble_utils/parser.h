#ifndef PARSER_H
#define PARSER_H

void writeFile(uint32_t *contents,int num_of_instructions ,char *filename);
void freeTokens(char **tokens, int *N);
char **tokenizer(char *line, int *N);
void preReadCodes(map *symbol_table);
int isLabel(char* token);

#endif