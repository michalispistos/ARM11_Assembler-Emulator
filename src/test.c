#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char *word = "12345";
    char *word1 = "HELLO";
    char *word2 = "HELLOB";
    char *word3 = "BYE";

    printf("%d %d %d", !strcmp("HELLO", word1), !strcmp(word1, word2), !strcmp(word3, "BYE"));
    // 0, 1, 0
    // word++;
    printf("%d", 1 + atoi(++word));
}