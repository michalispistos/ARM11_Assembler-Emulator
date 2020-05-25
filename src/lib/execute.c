#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// VARIABLES TO HOLD REGISTER NUMBER OF PC AND CPSR
int PC = 13;
int CPSR = 14;

// RETURNS THE CONTENTS OF THE CPSR
int getCPSR(int *registers){
    return registers[CPSR];
}

// Enums that represent the bit positions of flags
enum CPSRflag{
    V = 28,
    C = 29,
    Z = 30,
    N = 31
};

// Enums to represent the condition codes
enum Conditions {
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
};


// Enums to represent the opcode for the instructions
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

// Returns the of an instruction
uint32_t condition(uint32_t instr){
    return instr >> 28;
}

// Checks if the condition of a instruction is met
int checkCondition(uint32_t instr, uint32_t *registers){
    uint32_t CPSR = registers[CPSR];

    int Zset = !((CPSR >> Z) % 2);
    int NeqV = ((CPSR >> N) % 2) == ((CPSR >> V) % 2);

    switch (condition(instr))
    {
    case EQ:
        return Zset;
    case NE:
        return !Zset;
    case GE:
        return NeqV;
    case LT:
        return !NeqV;
    case GT:
        return !Zset && NeqV;
    case LE:
        return Zset || NeqV; 
    default:
        return 1;
    }
}

// Creates a mask that will show the n LSBs
uint32_t mask2(int no_of_bits){
    return (1 << no_of_bits) - 1;
}

/////////////////////////////////////////////////////////////////////



// DATA PROCESSING
void executeDataProcess(int *registers, uint32_t instr){
    int cond = instr >> 28;
    int I = (instr >> 25) % 2;
    int opcode = (instr >> 21) & mask2(4);
    int S = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask2(4);
    int Rd = (instr >> 12) & mask2(4);
    int operand = instr & mask2(12);

    // SET OPERAND2 VALUE
    uint32_t operand2;
    int carryOut;
    if (I){
       uint32_t immediate = (operand & 255);
       int rotate = (operand >> 8) * 2;
        operand2 = (immediate >> rotate)|(immediate << (32 - rotate));
    } else{
        //follow instructions for I not set
        int shift = operand >> 4;
        int Rm = operand & 15;
        int shiftType = (shift >> 1) & 3;
        int shiftAmount;
        if(shift & 1){
            int Rs = shift >> 4;
            int shiftAmount = registers[Rs] & 255;
        } else {
            shiftAmount = (shift >> 3);
        }
        switch (shiftType)
        {
            case 0:
                operand2 = Rm << shiftAmount;
                carryOut = (Rm >> shiftAmount + 1) & 1;
                break;
            case 1:
            // NOT SURE IF THE CARRYOUT IS MEANT TO TBE LSB OF THE DISCARDED BIT FOR RIGHT SHIFT
                operand2 = Rm >> shiftAmount;
                carryOut = Rm & 1;
                break;    
            case 2:
                // should take msb of Rm and fill in the 0s from the shift with it. Arithmetic right shift
                if (Rm >> 31){
                    operand2 = (mask2(32-shiftAmount) << (shiftAmount)) & (Rm >> shiftAmount);
                } else {
                    operand2 = Rm >> shiftAmount;
                }
                break;
            case 4:
                operand2 = (Rm >> shiftAmount)|(Rm << (32 - shiftAmount));
                break;
            default:
                printf("ERROR! shiftType is %d",shiftType);

        }

    }


    // INTERPRETATION OF OPCODE
    uint32_t result;
    int newC;
    switch (opcode)
    {
    case AND:
        result = Rn & operand2;
        registers[Rd] = result;
        newC = carryOut;
        break;
    case EOR:
        result = Rn ^ operand2;
        registers[Rd] = result;
        newC = carryOut;
        break;
    case SUB:
        result = Rn - operand2;
        registers[Rd] = result;
        // newC = result >> 31;? 
        break;
    case RSB:
        result = operand2 - Rn;
        registers[Rd] = result;
        // newC = result >> 31;?
        break;
    case ADD:
        result = Rn + operand2;
        registers[Rd] = result;
        // newC = result >> 31;?
        break;
    case TST:
        result = Rn & operand2;
        newC = carryOut;
        break;
    case TEQ:
        result = Rn ^ operand2;
        newC = carryOut;
        break;
    case CMP:
        result = Rn - operand2;
        // newC = result >> 31;?
        break;
    case ORR:
        result = Rn | operand2;
        registers[Rd] = result;
        newC = carryOut;
        break;
    case MOV:
        result = operand2;
        registers[Rd] = result;
        newC = carryOut;
        break;
    default:
        printf("ERROR! Opcode is: %d", opcode);
    }


    // if S is set then CPSR flags are changed
    if (S){
        int newN = (getCPSR(registers) >> N) & 1;
        int newZ = result & 0;
        int newC;
        int newCPSR = ((newN << 3) & (newZ << 2) & (newC << 1) & V) << 28; 
        registers[CPSR] = ((getCPSR(registers) >> 4) << 4) & newCPSR;
    }

}


/////////////////////////////////////////////////////////////

// MULTIPLY
// PC not used as an operand or destination register
// Rd will not be equal to Rm
void executeMultiply(int* registers, uint32_t instr){
    int cond = condition(instr);
    int A = (instr >> 21) % 2;
    int S = (instr >> 20) % 2;
    int Rd = (instr >> 16) & mask2(4);
    int Rn = (instr >> 12) & mask2(4);
    int Rs = (instr >> 8) & mask2(4);
    int Rm = instr & mask2(4);
    uint32_t result;

    printf("cond: %d\n", cond);
    printf("A; %d\n", A);
    printf("S: %d\n", S);
    printf("Rd: %d\n", Rd);
    printf("Rn: %d\n", Rn);
    printf("Rs: %d\n", Rs);
    printf("Rm: %d\n",Rm);

    uint32_t nFlag = cond >> 3;
    uint32_t zFlag = (cond >> 2 & 01);
    uint32_t cFlag = (cond >> 1 & 001);
    uint32_t vFlag = (cond & 0001);
    


    if(nFlag || zFlag || cFlag || vFlag){ 
    if (A) {
        //mutiply and accumalate
        result = Rm * Rs + Rn;
    } else {
        //multiply
        result = Rm * Rs;
    }

          

    // Rd = to the lower 32 bits of the result?
     registers[Rd] = result;      

    if (S) {
        //N and Z flags of CPSR should be updated
        //N is set to bit 31 of the result and Z is set iff the result is        
        uint32_t newNFlag = result>>31;
        uint32_t newZFlag; 
        if(result == 0){
          newZFlag = 1;
        }else{
          newZFlag = 0;
        }
        nFlag = newNFlag;
        zFlag = newZFlag;
    }

    //After modifying the flags(if we have to) then we have to set register[16]
    //So it carries the values of the current flags
    registers[16] = (nFlag << 3) + (zFlag << 2) + (cFlag<<1) + (vFlag);
  }
}


///////////////////////////////////////////////////////////////////

void executeSingleDataTransfer(int *registers, uint32_t instr){
    int cond = condition(instr);
    int I = (instr >> 25) % 2;
    int P = (instr >> 24) % 2;
    int U = (instr >> 23) % 2;
    int L = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask2(4);
    int Rd = (instr >> 12) & mask2(4);
    int offset = instr & mask2(12);

    if (I){
        // OFFSET IS SHIFTED REGISTER
    } 
}


///////////////////////////////////////////////////////////////////


void executeBranch(int *registers, uint32_t instr){
    int cond = condition(instr);
    int offset = instr & mask2(24);
}

