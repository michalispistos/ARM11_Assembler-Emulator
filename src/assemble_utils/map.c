#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Map {
    char word[511]; // From spec
    uint32_t code; // Can either be the instruction code or instruction address
    struct Map *next;
} map;


// Creates a new element
map *createMap(const char *word, uint32_t code){
    map *res = (map *) malloc(sizeof(map));
    if (!res){
        perror("createMap, NULL pointer allocated");
        exit(EXIT_FAILURE);
    }

    strcpy(res->word, word);
    res->code = code;
    return res;
}

// Adds item to the front of the data structure
map *addMap(map *root, const char *word, uint32_t code){
    map *new = createMap(word, code);
    if(root){
        new->next = root;
    }
    return new;
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
void printCode(const char *word, const uint32_t code){
    printf("[0x%08x] ", code);
}

// Testing map using main - seems to word
int main(void){
    map *root = createMap("A", 1);
    root = addMap(root, "B", 16);
    root = addMap(root, "C", 256);
    functionMap(root, printCode);
    destroyMap(root);
    return EXIT_SUCCESS;
}