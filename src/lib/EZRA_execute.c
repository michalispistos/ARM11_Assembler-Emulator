#include <stdint.h>

int PC = 13;
int CPSR = 14;

int getCPSR(int *registers){
    return registers[CPSR];
}

enum CPSRflag{
    V = 28,
    C = 29,
    Z = 30,
    N = 31
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

uint32_t condition(uint32_t instr){
    return instr >> 28;
}

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

uint32_t mask(int no_of_bits){
    return (1 << no_of_bits) - 1;
}

void data_proc(uint32_t instr, int *registers, int *memory){
    int cond = condition(instr);
    int I = (instr >> 25) % 2;
    int opcode = (instr >> 21) & mask(4);
    int S = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask(4);
    int Rd = (instr >> 12) & mask(4);
    int operand = instr & mask(12);

    int result = 0;
    if (checkCondition(instr, registers)){

    }

    if (S){
        if !(result){
            registers[CPSR] |= 5;
        }
    }
}

int main(void){
}