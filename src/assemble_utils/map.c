#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "map.h"


// Creates a new element
map *create_map(){
  map *res = (map *) calloc(1, sizeof(map));
  if (!res){
    perror("createMap, NULL pointer allocated");
    exit(EXIT_FAILURE);
  }

  /*
    strcpy(res->word, word);
    res->code = code;
  */
  return res;
}

// Adds item to the front of the data structure
void add_map(map *root, const char *word, uint32_t code, assemble_function function){
  if(!(root->word[0])){
    strcpy(root->word,word);
    root->code = code;
    root->function = function;
    return; 
  }
  if (root->next){
    add_map(root->next, word, code, function);
    return;
  }

  map *new = create_map();
  new->code = code;
  strcpy(new->word,word);
  new->function = function;
  root->next = new;
  //if(root){
  //    new->next = root;
  //}
  //return new;
}

// Applies a function to each element in a map
void function_map(map *root, void (*func)(const char *, const uint32_t)){
  if (root){
    func(root->word, root->code);
    function_map(root->next, func);
  }
}

// Destroys a map
void destroy_map(map *elem){
  if (elem){
    destroy_map(elem->next);
    free(elem);
  }
}

// Prints the word an element contains
void print_word(const char *word, const uint32_t code){
  printf("[%s] ", word);
}

// Prints the code an element contains
void print_code(const char* word,const uint32_t code){
  printf("%d", code);
}

void print_map(map* root){
  if(root){
    printf("%s-%u\n",root->word,root->code);
    print_map(root->next);
  }
}

void set_code(map* root, char* word, uint32_t code) {
  if(!root) {
    perror("Set code failed");
    exit(EXIT_FAILURE);
  }
  if (!strcmp(root->word,word)) {
    root->code = code;
    return;
  } else {
    set_code(root->next,word,code);
  }
}

uint32_t get_code(const map* root, char* word){
  if (!root) return 0;
  if(!strcmp(root->word,word)){
    return root->code;
  }

  return get_code(root->next,word);
}

assemble_function get_function(map* root, char* word){
  if (!strcmp(root->word,word)){
    return root->function;
  }
  return get_function(root->next,word);
}

uint32_t test(char *token,uint32_t code){
  return code;
}

void set_function(map *root, char* word, assemble_function function){
  if(!strcmp(root->word,word)){
    root->function = function;
  } else {
    set_function(root->next, word, function);
  }
}

map *get_map_from_word(map *root, const char *word){
  // Will return the first map element with same word
  // Returns NULL if this does not exist

  map *curr = root;
  while(curr && strcmp(curr->word, word)){
    curr = curr->next;
  }
  return curr;
}

// Testing map using main - seems to work
/*
  int main(void){
  map *root = createMap();
  add_map(root,"A",1,NULL);
  add_map(root, "B", 16,NULL);
  add_map(root, "C", 256,NULL);
  char *token = calloc(1,sizeof(char));
  strcpy(token,"A");
  char *word = calloc(1,sizeof(char));
  strcpy(word,"A");
  set_function(root,word,test);
  assemble_function func = (get_function(root,word));
  uint32_t res = (func)(token,1);
  printf("%u\n",res);
  printMap(root);
  destroy_map(root);
  return EXIT_SUCCESS;
    
  }
*/


