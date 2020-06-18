#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "emulate_utils/initialise.h"
#include "emulate_utils/pipeline.h"

#define PC (15)

void emulate(uint32_t *registers, uint32_t *memory)
{
  uint32_t instrA = 1; //fetching
  uint32_t instrB = 1; //decoding
  uint32_t decoded = 5;
  int counter = 0;

  do
  {
    if (counter > 1)
    {
      decoded = decode(instrB);
      if (decoded == 0)
      {
        break;
      }
      if (execute(decoded, instrB, registers, memory) == 1 && decoded == 4)
      {
        instrB = fetch(registers, memory);
        registers[PC] += 4;
        instrA = fetch(registers, memory);
        registers[PC] += 4;
        continue;
      }
    }
    instrB = instrA;
    instrA = fetch(registers, memory);
    registers[PC] += 4;
    counter++;
  } while (decoded != 0);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Incorrect number of arguments!\n");
    return EXIT_FAILURE;
  }

  uint32_t *memory = initialize_memory();
  uint32_t *registers = start_registers();

  load_file(memory, argv[1]);

  emulate(registers, memory);

  print_registers(registers);
  print_memory_hex(memory);

  free_registers(registers);
  free_memory(memory);
  return EXIT_SUCCESS;
}