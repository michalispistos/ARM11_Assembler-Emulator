#ifndef INITIALISE_H
#define INITIALISE_H

#define num_of_registers (17)
#define memory_capacity (65536)

//initialises registers
uint32_t *start_registers(void);

//frees the registers
void free_registers(uint32_t *registers);

//frees the memory
void free_memory(uint32_t *memory);

//prints the registers
void print_registers(uint32_t *registers);

//reads in binary file into given array
void load_file(uint32_t *memory,char *filename);

//initialises memory array and returns
uint32_t* initialize_memory();

//prints memory
void print_memory(uint32_t *memory);

#endif