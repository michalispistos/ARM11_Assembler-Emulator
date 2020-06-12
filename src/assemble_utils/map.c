#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "map.h"


map *create_map(){
  map *res = (map *) calloc(1, sizeof(map));
  if (!res){
    perror("createMap, NULL pointer allocated");
    exit(EXIT_FAILURE);
  }
  strcpy(res->word,"HEAD");
  res->code = 0;
  return res;
}

void add_map(map *root, const char *word, uint32_t code, assemble_function function){
  if (!strcmp(root->word, "HEAD")){
    strcpy(root->word, word);
    root->code = code;
    root->function = function;
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
}

void destroy_map(map *elem){
  if (elem) {
    destroy_map(elem->next);
    free(elem);
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

void set_function(map *root, char* word, assemble_function function){
  if(!strcmp(root->word,word)){
    root->function = function;
  } else {
    set_function(root->next, word, function);
  }
}

map *get_map_from_word(map *root, const char *word){
  map *curr = root;
  while(curr && strcmp(curr->word, word)){
    curr = curr->next;
  }
  return curr;
}




