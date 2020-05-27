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


  uint32_t instrA = 2; // fetching
  uint32_t instrB = 2; // decoding
  uint32_t instrC = 2; //executing
  int counter = 0;
  int tempcounter = 0;
  uint32_t decoded = 5;

  // FETCH DECODE EXECUTE CYCLE

  do {
    if (tempcounter>2 && counter > 1){
      execute(decoded, instrC,registers,memory);
      if (checkCondition(instrC,registers) && decoded == 4){
        instrB = instrA;
        counter = 1;
        continue;
      }
    }
    if (counter > 0){
      decoded = decode(instrB);
    }
    instrC = instrB;
    instrB = instrA;
    if (instrA != 0){
      instrA = fetch(registers,memory);
    }
    if (instrC != 0){
      registers[15] += 4;
    }
    counter++;
    tempcounter++;
  } while (instrC != 0);
  
  printRegisters(registers);
  printMemoryHex(memory);
}
