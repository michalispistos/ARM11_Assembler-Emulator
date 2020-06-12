#ifndef MAP_H
#define MAP_H

#define MAX_CHAR_LENGTH (511)

typedef struct Map map;

typedef uint32_t (*assemble_function) (map *symbols, char **tokens, int N, uint32_t code);


struct Map {
  char word[MAX_CHAR_LENGTH]; // From spec
  uint32_t code; // Can either be the instruction code or instruction address
   assemble_function function; // IF NULL, IT IS A LABEL
  struct Map *next;
};




map *create_map();
void add_map(map *root, const char *word, uint32_t code, assemble_function function);
void print_map(map* root);
assemble_function get_function(map* root, char* word);
void set_function(map *root, char* word, assemble_function function);
uint32_t get_code(const map* root, char* word);
void set_code(map* root, char* word, uint32_t code);
map *get_map_from_word(map *root, const char *word);
void destroy_map(map *elem);

#endif