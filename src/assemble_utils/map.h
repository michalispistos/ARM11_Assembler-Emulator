#ifndef MAP_H
#define MAP_H

#define MAX_WORD_LENGTH (511)
#define MAX_INSTRUCTIONS (10)

typedef struct Map_node map_node;

typedef struct Map{
  map_node *header;
  map_node *stored_expressions;
  uint32_t end;
} map;

typedef uint32_t (*assemble_function) (map *symbols, char **tokens, int N, uint32_t code);

struct Map_node {
  char word[MAX_WORD_LENGTH]; // From spec
  uint32_t code; // Can either be the instruction code or instruction address
  assemble_function function; // IF NULL, IT IS A LABEL
  map_node *next;
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

// adds item to end of a chain of nodes
void add_map_node(map_node *node, const char *word, uint32_t code, assemble_function function);

// Returns the first map element with same word
// Returns NULL if this does not exist
map_node *get_map_node_from_word(map *root, const char *word);

// Destroys a map
void destroy_map(map *elem);

#endif