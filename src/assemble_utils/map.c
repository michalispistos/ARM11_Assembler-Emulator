#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"

map_node *create_map_node()
{
  map_node *new_map_node = (map_node *)calloc(1, sizeof(map_node));
  if (!new_map_node)
  {
    perror("create_map_node, NULL pointer allocated");
    exit(EXIT_FAILURE);
  }
  return new_map_node;
}

map *create_map()
{
  map *new_map = (map *)calloc(1, sizeof(map));
  if (!new_map)
  {
    perror("create_map, NULL pointer allocated");
    exit(EXIT_FAILURE);
  }
  new_map->header = create_map_node();
  new_map->stored_expressions = create_map_node();
  return new_map;
}

void add_map_node(map_node *node, const char *word, uint32_t code, assemble_function function)
{
  if (node->next)
  {
    add_map_node(node->next, word, code, function);
    return;
  }
  map_node *new_map_node = create_map_node();
  assert(new_map_node);
  strcpy(new_map_node->word, word);
  new_map_node->code = code;
  new_map_node->function = function;
  node->next = new_map_node;
}

void add_map(map *root, const char *word, uint32_t code, assemble_function function)
{
  assert(root);
  add_map_node(root->header, word, code, function);
}

void destroy_map_node(map_node *node)
{
  /*
  map_node* temp = node->next;
  free(node);
  if(temp){
  destroy_map_node(temp);
  }
  */
  if (node->next)
  {
    destroy_map_node(node->next);
    //free(node);
  }
  free(node);
}

void destroy_map(map *elem)
{
  assert(elem);
  destroy_map_node(elem->header);
  destroy_map_node(elem->stored_expressions);
  free(elem);
}

void set_code_node(map_node *node, char *word, uint32_t code)
{
  if (!node)
  {
    perror("Function set_code failed. Could not find node with word.");
    exit(EXIT_FAILURE);
  }
  if (!strcmp(node->word, word))
  {
    node->code = code;
  }
  else
  {
    set_code_node(node->next, word, code);
  }
}

void set_code(map *root, char *word, uint32_t code)
{
  assert(root);
  set_code_node(root->header, word, code);
}

uint32_t get_code_node(map_node *node, char *word)
{
  if (!node)
  {
    perror("Function get_code failed. Could not find node with word.");
    exit(EXIT_FAILURE);
  }
  if (!strcmp(node->word, word))
  {
    return node->code;
  }
  return get_code_node(node->next, word);
}

uint32_t get_code(const map *root, char *word)
{
  assert(root);
  return get_code_node(root->header, word);
}

assemble_function get_function_node(map_node *node, char *word)
{
  if (!node)
  {
    perror("Function get_function failed. Could not find node with word.");
    exit(EXIT_FAILURE);
  }
  if (!strcmp(node->word, word))
  {
    return node->function;
  }
  return get_function_node(node->next, word);
}

assemble_function get_function(map *root, char *word)
{
  assert(root);
  return get_function_node(root->header, word);
}

void set_function_node(map_node *node, char *word, assemble_function function)
{
  if (!node)
  {
    perror("Function set_function failed. Could not find node with word.");
    exit(EXIT_FAILURE);
  }
  if (!strcmp(node->word, word))
  {
    node->function = function;
  }
  else
  {
    set_function_node(node->next, word, function);
  }
}

void set_function(map *root, char *word, assemble_function function)
{
  assert(root);
  set_function_node(root->header, word, function);
}

// returns NULL if cannot find map_node
map_node *get_map_node_from_word(map *root, const char *word)
{
  map_node *curr = root->header->next;
  while (curr && strcmp(curr->word, word))
  {
    curr = curr->next;
  }
  return curr;
}
