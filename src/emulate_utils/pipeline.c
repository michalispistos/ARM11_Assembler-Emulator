#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pipeline.h"
#include "pipeline_utils.h"

// Enum of possible commands in the ARM project
enum Commands {
  HALT,
  DATA_PROC,
  MULTIPLY,
  SDTRANS,
  BRANCH
};

uint32_t fetch(uint32_t * registers, uint32_t * memory) {
  uint32_t PC_value = registers[PC];
  uint32_t value = 0;
  for (int i = 3; i >= 0; i--) {
    value += memory[PC_value + i] << (8 * i);
  }
  return value;
}

uint32_t decode(uint32_t instr) {
  if (instr == 0) {
    return HALT;
  } else if (1 & instr >> 27) {
    return BRANCH;
  } else if (1 & instr >> 26) {
    return SDTRANS;
  } else if ((((instr >> 4) & mask(4)) == 9) && !((instr >> 22) & mask(6))) {
    return MULTIPLY;
  }

  return DATA_PROC;
}

int execute(uint32_t decoded, uint32_t instr, uint32_t * registers, uint32_t * memory) {
  if (check_condition(instr, registers)) {
    switch (decoded) {
    case HALT:
      break;
    case DATA_PROC:
      execute_data_process(registers, instr);
      break;
    case MULTIPLY:
      execute_multiply(registers, instr);
      break;
    case SDTRANS:
      execute_single_data_transfer(registers, memory, instr);
      break;
    case BRANCH:
      execute_branch(registers, instr);
      break;
    default:
      printf("should not be here, value of decoded: %u\n", decoded);
      break;
    }
    return 1;
  }
  return 0;
}