#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "map.h"


// Creates a new element
map *createMap(){
  map *res = (map *) malloc(sizeof(map));
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
void addMap(map *root, const char *word, uint32_t code, assemble_function function){
  if(!(root->word[0])){
    strcpy(root->word,word);
    root->code = code;
    root->function = function;
    return; 
  }
  map* new = createMap();
  strcpy(new->word,word);
  new->code = code;
  if (root->next){
    new->next = root->next;
    root->next = new;
  } else {
    root->next = new;
  }
  //if(root){
  //    new->next = root;
  //}
  //return new;
}

// Applies a function to each element in a map
void functionMap(map *root, void (*func)(const char *, const uint32_t)){
  if (root){
    func(root->word, root->code);
    functionMap(root->next, func);
  }
}

// Destroys a map
void destroyMap(map *elem){
  if (elem){
    destroyMap(elem->next);
    free(elem);
  }
}

// Prints the word an element contains
void printWord(const char *word, const uint32_t code){
  printf("[%s] ", word);
}

// Prints the code an element contains
void printCode(const char* word,const uint32_t code){
  printf("%d", code);
  //[0x%08x]
}

void printMap(map* root){
  if(root){
    printf("%s-%u\n",root->word,root->code);
    printMap(root->next);
  }
}

uint32_t getCode(const map* root, char* word){
  if(!strcmp(root->word,word)){
    return root->code;
  }

  return getCode(root->next,word);
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

// Testing map using main - seems to work
/*
  int main(void){
  map *root = createMap();
  addMap(root,"A",1,NULL);
  addMap(root, "B", 16,NULL);
  addMap(root, "C", 256,NULL);
  char *token = calloc(1,sizeof(char));
  strcpy(token,"A");
  char *word = calloc(1,sizeof(char));
  strcpy(word,"A");
  set_function(root,word,test);
  assemble_function func = (get_function(root,word));
  uint32_t res = (func)(token,1);
  printf("%u\n",res);
  printMap(root);
  destroyMap(root);
  return EXIT_SUCCESS;
    
  }
*/


