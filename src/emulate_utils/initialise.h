#ifndef INITIALISE_H
#define INITIALISE_H


uint32_t *start_registers(void);
void free_registers(uint32_t *registers);
void free_memory(uint32_t *memory);
void print_registers(uint32_t *registers);
void load_file(uint32_t *memory,char *filename);
uint32_t* initialize_memory();
void print_memory_hex(uint32_t *memory);

#endif