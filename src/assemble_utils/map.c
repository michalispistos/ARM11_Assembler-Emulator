#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_CHAR_LENGTH (511)

typedef struct Map {
    char word[MAX_CHAR_LENGTH]; // From spec
    uint32_t code; // Can either be the instruction code or instruction address
    struct Map *next;
} map;


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
void addMap(map *root, const char *word, uint32_t code){
    if(!(root->word[0])){
       strcpy(root->word,word);
       root->code = code;
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

uint32_t getCode(map* root, char* word){
    if(root->word == word){
        return root->code;
    }

    return getCode(root->next,word);
}

// Testing map using main - seems to work
/*
int main(void){
    map *root = createMap();
    root = addMap(root,"A",1);
    root = addMap(root, "B", 16);
    root = addMap(root, "C", 256);
    printMap(root);
    destroyMap(root);
    return EXIT_SUCCESS;
    
}
*/