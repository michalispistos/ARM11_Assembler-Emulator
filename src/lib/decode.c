#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*  Can take an instruction and display the various values depending on 
    the type of instruction. Also added a function to determine the type
    of instruction but still needs to be tested.

    NEED TO DO:
        - Agree on suitable format to output for execution
        - Neaten code that determines type (magic numbers and shifts etc)
        - Change to uint_32 for all to prevent any errors?
*/

int mask(int no_of_bits){
    return (1 << no_of_bits) - 1;
}

enum Commands {
    HALT,
    DATA_PROC,
    MULTIPLY,
    SDTRANS,
    BRANCH
};

enum Conditions {
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
};

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

int decode(int instr){
    if (instr == 0){
        return HALT;
    } 

    if (1 & instr >> 27){
        return BRANCH;
    }
    
    if (1 & instr >> 26){
        return SDTRANS;
    }

    // NOT QUITE SURE IF THIS WORKS
    if ((((1 << 4) - 1) & (instr >> 4) == 10) && !(((1 << 6) - 1) & (instr >> 22))){
        return MULTIPLY;
    }

    return DATA_PROC;
}

int condition(uint32_t instr){
    return instr >> 28;
}

void halt(void){

}

void data_proc(uint32_t instr){
    int cond = condition(instr);
    int I = (instr >> 25) % 2;
    int opcode = (instr >> 21) & mask(4);
    int S = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask(4);
    int Rd = (instr >> 12) & mask(4);
    int operand = instr & mask(12);

    printf("%d\n", cond);
    printf("%d\n", I);
    printf("%d\n", opcode);
    printf("%d\n", S);
    printf("%d\n", Rn);
    printf("%d\n", Rd);
    printf("%d\n", operand);
} 

void multiply(uint32_t instr){
    int cond = condition(instr);
    int A = (instr >> 21) % 2;
    int S = (instr >> 20) % 2;
    int Rd = (instr >> 16) & mask(4);
    int Rn = (instr >> 12) & mask(4);
    int Rs = (instr >> 8) & mask(4);
    int Rm = (instr >> 8) & mask(4);

    printf("%d\n", cond);
    printf("%d\n", A);
    printf("%d\n", S);
    printf("%d\n", Rd);
    printf("%d\n", Rn);
    printf("%d\n", Rs);
    printf("%d\n",Rm);
}

void sdt(uint32_t instr){
    int cond = condition(instr);
    int I = (instr >> 25) % 2;
    int P = (instr >> 24) % 2;
    int U = (instr >> 23) % 2;
    int L = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask(4);
    int Rd = (instr >> 12) & mask(4);
    int offset = instr & mask(12);

    printf("%d\n", cond);
    printf("%d\n", I);
    printf("%d\n", P);
    printf("%d\n", U);
    printf("%d\n", L);
    printf("%d\n", Rn);
    printf("%d\n", Rd);
    printf("%d\n", offset);
}

void branch(uint32_t instr){
    int cond = condition(instr);
    int offset = instr & mask(24);

    printf("%d\n", cond);
    printf("%d\n", offset);
}

int main(void){
    data_proc(0xA3CE7F0A);
    return 0;
}
