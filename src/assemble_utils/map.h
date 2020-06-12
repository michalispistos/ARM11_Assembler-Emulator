#ifndef MAP_H
#define MAP_H

#define MAX_WORD_LENGTH (511)

typedef struct Map map;

typedef uint32_t (*assemble_function) (map *symbols, char **tokens, int N, uint32_t code);

struct Map {
  char word[MAX_WORD_LENGTH]; // From spec
  uint32_t code; // Can either be the instruction code or instruction address
  assemble_function function; // IF NULL, IT IS A LABEL
  struct Map *next;
};

//creates a map
map *create_map();

// Adds item to the front of the data structure
void add_map(map *root, const char *word, uint32_t code, assemble_function function);

//gets function
assemble_function get_function(map* root, char* word);

//sets function
void set_function(map *root, char* word, assemble_function function);

//gets code
uint32_t get_code(const map* root, char* word);

//sets code
void set_code(map* root, char* word, uint32_t code);

// Returns the first map element with same word
// Returns NULL if this does not exist
map *get_map_from_word(map *root, const char *word);

// Destroys a map
void destroy_map(map *elem);

#endif