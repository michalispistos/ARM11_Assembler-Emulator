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


 uint32_t instrA = 1; // fetching
 uint32_t instrB = 1; // decoding

 int counter = 0;
 uint32_t decoded = 5;


  int tempcounter = 0;
  uint32_t instrC = 1; //executing
  
  do {
    if (tempcounter > 2 && counter > 1){
      //printf("INSTRC IS %x\n",instrC);
      execute(decoded, instrC,registers,memory);
      if (decoded == 0){
        break;
      }
      if (checkCondition(instrC,registers) && decoded == 4){
        counter = 1;
        decoded = 5;
        instrB = 1;
        registers[15] -= 4;
        instrA = fetch(registers,memory);
        registers[15] += 4;
        //printf("fetching in checkcond, FETCHED IS %x\n",instrA);
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
     // printf("fetching inside instrA!=0, fetched is: %x\n", instrA);
    }
    
    if (instrC != 0){
      registers[15] += 4;
    }
  
    counter++;
    tempcounter++;
    //printf("INSTRA is %x, INSTRB is %x, INSTRC is %x\n", instrA, instrB, instrC);
    //printRegisters(registers);
  } while (1);
  
  
  /*
  
  do {
    if (counter > 0){
      decoded = decode(instrB);
      if(decoded == 0){
        break;
      }
      execute(decoded,instrB,registers,memory);
      if (checkCondition(instrB,registers) && decoded == 4){
        counter = 0;
        instrB = 1;
        continue;
      }
    }
    instrB = instrA;
    if (instrA !=0){
      instrA = fetch(registers,memory);
    }

    registers[15] += 4;
    counter++;
  } while (1);

*/
  

  
  printRegisters(registers);
  printMemoryHex(memory);
  freeRegisters(registers);
  freeMemory(memory);
}
