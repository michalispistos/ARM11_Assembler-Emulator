#include <stdint.h>

uint32_t startRegisters(void);
void freeRegisters(uint32_t *registers);
void freeMemory(uint32_t *memory);
void printRegisters(void);
void loadFile(uint32_t *memory,char *filename);
uint32_t* initializeMemory();
void printMemoryHex(uint32_t *memory);
