#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
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
  uint32_t instrA = NULL; // fetching
  uint32_t instrB = NULL; // decoding
  uint32_t instrC = NULL; //executing
  uint32_t decoded;
  printf("R3 = %d\n", registers[3]);

  // FETCH DECODE EXECUTE CYCLE

  do {
    if (instrC != NULL){
      execute(decoded, instrC,registers,memory);
    }
    if (instrB != NULL){
      decoded = decode(instrB);
    }
    instrC = instrB;
    instrB = instrA;
    instrA = fetch(registers,memory);
    registers[15] += 4;

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

