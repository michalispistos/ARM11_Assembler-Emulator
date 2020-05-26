#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "execute.h"

// CONSTANTS TO HOLD REGISTER NUMBER OF PC AND CPSR
const int PC = 15;
const int CPSR = 16;

// Enums that represent the bit positions of flags
enum CPSRflag{
    V = 28,
    C = 29,
    Z = 30,
    N = 31
};

// Enum of possible commands in the ARM project
enum Commands {
    HALT,
    DATA_PROC,
    MULTIPLY,
    SDTRANS,
    BRANCH
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
static uint32_t condition(uint32_t instr){
    return instr >> 28;
}

// Checks if the condition of a instruction is met
static int checkCondition(uint32_t instr, uint32_t *registers){
    uint32_t CPSR_val = registers[CPSR];
    int Zset = !((CPSR_val >> Z) % 2);
    int NeqV = (((CPSR_val >> N) % 2) == ((CPSR_val >> V) % 2));
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
    case AL:
        return 1;
    default: 
        return 0;
    }
}

// Creates a mask that will show the n LSBs
static uint32_t mask(int no_of_bits){
    return (1 << no_of_bits) - 1;
}

// Returns the result if operand2 interpreted as immediate value
static uint32_t immediateVal(int operand2){
    uint32_t result = operand2 & mask(8);
    uint32_t rotateTimes = (operand2 >> 8) * 2;
    result = (result >> rotateTimes) | ((result & mask(rotateTimes)) << (32 - rotateTimes));
    return result; 
}

// Returns the result if operand2 interpreted as a register
static uint32_t registerOper(int operand2, int S, uint32_t *registers){
    uint32_t result = registers[operand2 & mask(4)];
    int carry;
    // The contents of register Rm is not modified.
    uint32_t shift = operand2 >> 7;
    uint32_t shiftType = (operand2 >> 5) & mask(2);
    // if (!((operand2 >> 4) % 2)){
        switch (shiftType)
        {
        case 0:
            // Logical shift left
            carry = (result >> (32 - shift)) % 2;
            result <<= shift;
            break;
        case 1:
            // Logical shift right
            carry = (result >> (shift - 1)) % 2;
            result >>= shift;
            break;
        case 2:
            // Arithmetic shift right
            carry = (result >> (shift - 1)) % 2;
            int endVal = result >> 31;
            result >>= shift;
            result |= (endVal << 31);            
            break;
        case 3:
            // Rotate right
            carry = (result >> (shift - 1)) % 2;
            result = (result >> shift) | (result & mask(shift) << (32 - shift));
            break;
    // }
    }
    if (S){
        registers[CPSR] = carry ? registers[CPSR] | (1 << C) : registers[CPSR] & ~(1 << C);
    }
    return result;
}

/////////////////////////////////////////////////////////////////////

// DATA PROCESSING
static void executeDataProcess(uint32_t *registers, uint32_t instr){
    int I = (instr >> 25) % 2;
    int opcode = (instr >> 21) & mask(4);
    int S = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask(4);
    int Rd = (instr >> 12) & mask(4);
    int operand = instr & mask(12);

    // SET OPERAND2 VALUE
    uint32_t operand2;
    if (I){
       operand2 = immediateVal(operand);
    } else{
        //follow instructions for I not set
        operand2 = registerOper(operand,S,registers);
    }

    // INTERPRETATION OF OPCODE
    uint32_t result;
    switch (opcode)
    {
    case AND:
        result = registers[Rn] & operand2;
        registers[Rd] = result;
        break;
    case EOR:
        result = registers[Rn] ^ operand2;
        registers[Rd] = result;
        break;
    case SUB:
        result = registers[Rn] - operand2;
        registers[Rd] = result;
        break;
    case RSB:
        result = operand2 - registers[Rn];
        registers[Rd] = result;
        break;
    case ADD:
        result = registers[Rn] + operand2;
        registers[Rd] = result;
        break;
    case TST:
        result = registers[Rn] & operand2;
        break;
    case TEQ:
        result = registers[Rn] ^ operand2;
        break;
    case CMP:
        result = registers[Rn] - operand2;
        break;
    case ORR:
        result = registers[Rn] | operand2;
        registers[Rd] = result;
        break;
    case MOV:
        result = operand2;
        registers[Rd] = result;
        break;
    default:
        printf("ERROR! Opcode is: %d", opcode);
    }


    // if S is set then CPSR flags are changed
    if (S){
        int newN = (registers[CPSR] >> N) & 1;
        int newZ = result & 0;
        int newCPSR = ((newN << 3) & (newZ << 2) & V) << 28; 
        registers[CPSR] = ((registers[CPSR] >> 4) << 4) & newCPSR;
    }
}

/////////////////////////////////////////////////////////////

// MULTIPLY
// PC not used as an operand or destination register
// Rd will not be equal to Rm
static void executeMultiply(uint32_t* registers, uint32_t instr){
    int cond = condition(instr);
    int A = (instr >> 21) % 2;
    int S = (instr >> 20) % 2;
    int Rd = (instr >> 16) & mask(4);
    int Rn = (instr >> 12) & mask(4);
    int Rs = (instr >> 8) & mask(4);
    int Rm = instr & mask(4);
    uint32_t result;

    printf("cond: %d\n", cond);
    printf("A; %d\n", A);
    printf("S: %d\n", S);
    printf("Rd: %d\n", Rd);
    printf("Rn: %d\n", Rn);
    printf("Rs: %d\n", Rs);
    printf("Rm: %d\n",Rm);

    if (A) {
        //mutiply and accumalate
        result = registers[Rm] * registers[Rs] + registers[Rn];
    } else {
        //multiply
        result = registers[Rm] * registers[Rs];
    }

        
    //saving the result to destination register
     registers[Rd] = result;      

    // if S is set then CPSR flags are changed
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
        //After changing N,Z then we have to update register[16](CPSR)
        registers[16] = (newNFlag << 3) + (newZFlag << 2) + (registers[16]>>1 & 1) + (registers[16] & 1);
    }    
  }


///////////////////////////////////////////////////////////////////

// SINGLE DATA TRANSFER
static void executeSingleDataTransfer(uint32_t *registers, uint32_t *memory, uint32_t instr){
    int I = (instr >> 25) % 2;
    int P = (instr >> 24) % 2;
    int U = (instr >> 23) % 2;
    int L = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask(4);
    int Rd = (instr >> 12) & mask(4);
    int offset = instr & mask(12);

    if (checkCondition(instr, registers)){
        uint32_t interp_offset = I ? registerOper(offset, 0, registers) : immediateVal(offset);
        uint32_t result = Rn != PC ? registers[Rn] : registers[PC] + 8; 
        result += U ? interp_offset : - interp_offset;
        
        if (P){
            if (L){
                registers[Rd] = memory[result];
            } else {
                memory[result] = registers[Rd];
            }
        } else {
            if (L){
                registers[Rd] = memory[registers[Rn]];
            } else {
                memory[registers[Rn]] = registers[Rd];
            }
            registers[Rn] = result;
        }
    }
}


///////////////////////////////////////////////////////////////////


static void executeBranch(uint32_t *registers, uint32_t instr){
    uint32_t offset = instr & mask(24);
    
    /*
    Branch
    instructions contain a signed (2’s complement) 24 bit offset. This offset is shifted left 2 bits, sign extended
    to 32 bits and then added to the PC. Therefore, the branch instruction can specify a branch of +/- 32
    Mbytes. The offset will take into account the effect of the pipeline (i.e. PC is 8 bytes ahead of the
    instruction that is being executed).
    */

 
    offset = offset << 2;
    int firstBit = offset >> 25;
    if (firstBit){
        offset = offset | (mask(6)<<26);
    }
    registers[PC] += offset;
}


//////////////////////////////////////
// EXECUTE 


void execute(uint32_t decoded, uint32_t instr, uint32_t *registers, uint32_t* memory) {
    if(checkCondition(instr, registers)) {
        switch(decoded) 
        {
        case HALT:
            break;
        case DATA_PROC:
            executeDataProcess(registers, instr);
            break;
        case MULTIPLY:
            executeMultiply(registers, instr);
            break;
        case SDTRANS:
            executeSingleDataTransfer(registers, memory, instr);
            break;
        case BRANCH:
            executeBranch(registers, instr);
            break;
        default:
            printf("should not be here, value of decoded: %u\n", decoded);
            break;
    	 }
    }
}


