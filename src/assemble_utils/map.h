typedef struct Map map;
map *createMap();
map *addMap(map *root, const char *word, uint32_t code);
void printMap(map* root);