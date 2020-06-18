#ifndef PARSER_H
#define PARSER_H

void write_file(uint32_t *contents, int num_of_instructions, char *filename);
char **tokenizer(char *line, int *N);
void pre_read_codes(map *symbol_table);
int is_label(char *token);

#endif