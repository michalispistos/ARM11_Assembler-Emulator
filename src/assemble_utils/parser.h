#ifndef PARSER_H
#define PARSER_H

//writes binary instructions to a given file
void write_file(uint32_t *contents, int num_of_instructions, char *filename);

//tokenizes a assembly instruction into individual components
char **tokenizer(char *line, int *N);

//adds supported operations with their opcodes and respective assemble functions to given symbol table
void pre_read_codes(map *symbol_table);

//checks if a given token is a label
int is_label(char *token);

#endif