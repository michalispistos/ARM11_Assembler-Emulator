#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "initialise.h"

uint32_t *start_registers(void) {
  uint32_t *registers;
  registers = calloc(num_of_registers, 4);
  assert(registers != NULL);
  return registers;
}

void free_registers(uint32_t * registers) {
  free(registers);
}

void free_memory(uint32_t * memory) {
  free(memory);
}

void print_registers(uint32_t * registers) {
  printf("Registers:\n");
  for (int i = 0; i < 10; i++) {
    printf("$%d  :", i);
    printf(" %10d (0x%08x)\n", registers[i], registers[i]);
  }
  for (int i = 10; i < 13; i++) {
    printf("$%d :", i);
    printf(" %10d (0x%08x)\n", registers[i], registers[i]);
  }
  printf("PC  : %10u (0x%08x)\n", registers[15], registers[15]);
  printf("CPSR: %10d (0x%08x)\n", registers[16], registers[16]);
}

void load_file(uint32_t * memory, char * filename) {
  FILE * file = fopen(filename, "rb");
  assert(file != NULL);
  char * word = malloc(1);
  int i = 0;
  while (fread(word, 1, 1, file) == 1) {
    memory[i] = * word & 0xff;
    i++;
  }
  free(word);
  fclose(file);
}

uint32_t * initialize_memory() {
  uint32_t * memory;
  memory = calloc(memory_capacity, 1);
  assert(memory != NULL);
  return memory;
}

void print_memory_hex(uint32_t * memory) {
  printf("Non-zero memory:\n");
  for (int i = 0; i < 2048; i++) {
    uint32_t base = 4 * i;
    if (memory[base] | memory[base + 1] | memory[base + 2] | memory[base + 3]) {
      printf("0x%08x: 0x%02x%02x%02x%02x\n", base, memory[base], memory[base + 1], memory[base + 2], memory[base + 3]);
    }
  }
}
