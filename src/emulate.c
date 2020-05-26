#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/initialise.h"
#include "lib/fetch.h"
#include "lib/execute.h"
#include "lib/decode.h"

int main(int argc, char **argv){
  if (argc != 2) {
    printf("Incorrect number of arguments!\n");
    return EXIT_FAILURE;
  }

  uint32_t *memory = initializeMemory();
  uint32_t *registers = startRegisters();
  loadFile(memory,argv[1]);

  printMemoryHex(memory);
  printRegisters(registers);
  printf("________________\n");
  uint32_t instrA; // fetching
  uint32_t instrB; // decoding
  uint32_t instrC; //executing
  int counter = 0;
  uint32_t decoded;
  printf("R3 = %d\n", registers[3]);

  // FETCH DECODE EXECUTE CYCLE

  do {
    if (counter > 1){
      execute(decoded, instrC,registers,memory);
    }
    if (counter > 0){
      decoded = decode(instrB);
    }
    instrC = instrB;
    instrB = instrA;
    instrA = fetch(registers,memory);
    registers[15] += 4;
    counter++;
  } while (instrA != 0);
  execute(decoded,instrC,registers,memory);
  decoded = decode(instrB);
  execute(decoded,instrB,registers,memory);
  registers[15] += 4;
  
  //

  printMemoryHex(memory);
  printRegisters(registers);
  freeMemory(memory);
  freeRegisters(registers);
  return 0;
  
  }

