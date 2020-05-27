#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "decode.h"

/*  Can take an instruction and display the various values depending on 
    the type of instruction. Also added a function to determine the type
    of instruction but still needs to be tested.

    NEED TO DO:
        - Agree on suitable format to output for execution
        - Neaten code that determines type (magic numbers and shifts etc)
        - Change to uint_32 for all to prevent any errors?
*/


// Returns an integer mask of n 1s.
uint32_t mask2(int no_of_bits){
    return (1 << no_of_bits) - 1;
}

// Enum of possible commands in the ARM project
enum Commands {
    HALT,
    DATA_PROC,
    MULTIPLY,
    SDTRANS,
    BRANCH
};

// Possible condition conditions for the ARM Project.
// Condition codes are given in the 4 MSBs
enum Conditions {
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
};

// Possible operation codes for the ARM Project
// Only valid for a Data Processing instruction
enum OpCodes {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13,
};

/*  Decode takes an instruction which is a 32 bit unsigned integer
    We count from the LSB.
     It checks using the following method:
        - If 0, then halt
        - If bit 27 is 1, then it is a branching instruction
        - If bit 26 is 1, then it is a single data transfer instruction
        - If bits 7 to 4 are 1001, and bits 27 to 22 are all 0, it is
            a multiply instruction
        - Otherwise it must be a data processing instruction
*/
uint32_t decode(uint32_t instr){
    if (instr == 0){
        return HALT;
    } 

    else if (1 & instr >> 27){
        return BRANCH;
    }
    
    else if (1 & instr >> 26){
        return SDTRANS;
    }

    // NOT QUITE SURE IF THIS WORKS
    else if ((((instr >> 4) & mask2(4)) == 9) && !((instr >> 22) & mask2(6))){
        return MULTIPLY;
    }

    return DATA_PROC;
}

