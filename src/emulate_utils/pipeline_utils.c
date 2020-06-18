#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pipeline_utils.h"
#include "common_utils.h"

// Enums that represent the bit positions of flags
enum CPSRflag
{
    V = 28,
    C = 29,
    Z = 30,
    N = 31
};

// Enums to represent the condition codes
enum Conditions
{
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
};

// Enums to represent the opcode for the instructions
enum OpCodes
{
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

uint32_t condition(uint32_t instr)
{
    return instr >> 28;
}

int check_condition(uint32_t instr, uint32_t *registers)
{
    uint32_t CPSR_val = registers[CPSR];
    int Zset = ((CPSR_val >> Z) & 1);
    int NeqV = (((CPSR_val >> N) & 1) == ((CPSR_val >> V) & 1));
    uint32_t cond = condition(instr);
    switch (cond)
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
        return (!Zset) & NeqV;
    case LE:
        return Zset | !NeqV;
    case AL:
        return 1;
    default:
        return 0;
    }
}

// Returns the result if operand2 interpreted as immediate value
static uint32_t immediate_val(int operand2)
{
    uint32_t result = operand2 & mask(8);
    uint32_t rotateTimes = (operand2 >> 8) * 2;
    result = (result >> rotateTimes) | ((result & mask(rotateTimes)) << (32 - rotateTimes));
    return result;
}

// Returns the result if operand2 interpreted as a register
static uint32_t register_operand(int operand2, int S, uint32_t *registers, uint32_t *newC)
{
    uint32_t result = registers[operand2 & mask(4)];
    int carry = 0;

    // The contents of register Rm is not modified.
    uint32_t shift = operand2 >> 4;
    uint32_t shiftType = (shift >> 1) & mask(2);
    uint32_t shiftVal;

    if (shift & 1)
    {
        // shift specified by a register
        shiftVal = registers[shift >> 4] & mask(8);
    }
    else
    {
        // shift by a constant amount
        shiftVal = shift >> 3;
    }

    switch (shiftType)
    {
    case 0:
        // Logical shift left
        if (shiftVal != 0)
        {
            carry = (result >> (32 - shiftVal)) & mask(1);
        }
        else
        {
            carry = 0;
        }
        result <<= shiftVal;
        break;
    case 1:
        // Logical shift right
        if (shiftVal != 0)
        {
            carry = (result >> (shiftVal - 1)) % 2;
        }
        else
        {
            carry = 0;
        }
        result >>= shiftVal;
        break;
    case 2:
        // Arithmetic shift right
        if (shiftVal != 0)
        {
            carry = (result >> (shiftVal - 1)) % 2;
        }
        else
        {
            carry = 0;
        }
        int endVal = result >> 31;
        result >>= shiftVal;
        result |= (endVal << 31);
        break;
    case 3:
        // Rotate right
        if (shiftVal != 0)
        {
            carry = (result >> (shiftVal - 1)) % 2;
        }
        else
        {
            carry = 0;
        }
        result = (result >> shiftVal) | (result & mask(shiftVal) << (32 - shiftVal));
        break;
    }
    if (S)
    {
        *newC = carry & 1;
    }
    return result;
}

void execute_data_process(uint32_t *registers, uint32_t instr)
{
    int I = (instr >> 25) & 1;
    int opcode = (instr >> 21) & mask(4);
    int S = (instr >> 20) & 1;
    int Rn = (instr >> 16) & mask(4);
    int Rd = (instr >> 12) & mask(4);
    int operand = instr & mask(12);
    uint32_t newC;

    // SET OPERAND2 VALUE
    uint32_t operand2;
    if (I)
    {
        operand2 = immediate_val(operand);
    }
    else
    {
        //follow instructions for I not set
        operand2 = register_operand(operand, S, registers, &newC);
    }

    // INTERPRETATION OF OPCODE
    uint32_t result = 0;
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
        newC = ((int32_t)result >= 0);
        break;
    case RSB:
        result = operand2 - registers[Rn];
        registers[Rd] = result;
        newC = result >> 31;
        break;
    case ADD:
        result = registers[Rn] + operand2;
        registers[Rd] = result;
        newC = result >> 31;
        break;
    case TST:
        result = registers[Rn] & operand2;
        break;
    case TEQ:
        result = registers[Rn] ^ operand2;
        break;
    case CMP:
        result = registers[Rn] - operand2;
        newC = result >> 31;
        newC = ((int32_t)result >= 0);
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
    if (S)
    {
        uint32_t newN = result >> 31;
        uint32_t newZ = !(result ^ 0);
        uint32_t newV = (registers[CPSR] >> V) & 1;
        uint32_t newCPSR = (newN << 3) | (newZ << 2) | (newC << 1) | newV;
        registers[CPSR] = (newCPSR << 28) | (registers[CPSR] & mask(28));
    }
}


void execute_multiply(uint32_t *registers, uint32_t instr)
{
    int A = (instr >> 21) % 2;
    int S = (instr >> 20) % 2;
    int Rd = (instr >> 16) & mask(4);
    int Rn = (instr >> 12) & mask(4);
    int Rs = (instr >> 8) & mask(4);
    int Rm = instr & mask(4);
    uint32_t result;

    if (A)
    {
        //mutiply and accumalate
        result = registers[Rm] * registers[Rs] + registers[Rn];
    }
    else
    {
        //multiply
        result = registers[Rm] * registers[Rs];
    }

    //saving the result to destination register
    registers[Rd] = result;

    // if S is set then CPSR flags are changed
    if (S)
    {
        //N and Z flags of CPSR should be updated
        //N is set to bit 31 of the result and Z is set iff the result is zero
        uint32_t newNFlag = result >> 31;
        uint32_t newZFlag;
        if (result == 0)
        {
            newZFlag = 1;
        }
        else
        {
            newZFlag = 0;
        }
        //After changing N,Z then we have to update register[16](CPSR)
        registers[CPSR] = (newNFlag << N) | (newZFlag << Z) | (registers[CPSR] & mask(30));
    }
}

// to_memory takes a 32 bit instruction and stores it in the correct into memory
static void to_memory(uint32_t instr, uint32_t *memory, uint32_t address)
{
    for (int i = 0; i < 4; i++)
    {
        memory[address + i] = mask(8) & (instr >> 8 * i);
    }
}

// to_register returns a 32 bit word from memory (SAME AS FETCH)
static uint32_t to_register(uint32_t address, uint32_t *memory)
{
    uint32_t res = 0;
    for (int i = 3; i >= 0; i--)
    {
        res += memory[address + i] << (8 * i);
    }
    return res;
}

void execute_single_data_transfer(uint32_t *registers, uint32_t *memory, uint32_t instr)
{
    int I = (instr >> 25) % 2;
    int P = (instr >> 24) % 2;
    int U = (instr >> 23) % 2;
    int L = (instr >> 20) % 2;
    int Rn = (instr >> 16) & mask(4);
    int Rd = (instr >> 12) & mask(4);
    uint32_t offset = instr & mask(12);
    uint32_t interp_offset = I ? register_operand(offset, 0, registers, (uint32_t *)1) : immediate_val(offset);
    //if PC used as base register(Rn), then Rn must contain instruction's address plus 8 bytes
    uint32_t result = registers[Rn];
    if (P)
    {
        //offset is added/subtracted to base register before transferring data
        //will not change value of base register
        result += U ? interp_offset : -interp_offset;
        if (result == 0x20200000)
        {
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            registers[Rd] = result;
        }
        else if (result == 0x20200004)
        {
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            registers[Rd] = result;
        }
        else if (result == 0x20200008)
        {
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            registers[Rd] = result;
        }
        else if (result == 0x20200028)
        {
            printf("PIN OFF\n");
        }
        else if (result == 0x2020001c)
        {
            printf("PIN ON\n");
        }
        else if (result >= (1 << 16))
        {
            printf("Error: Out of bounds memory access at address 0x%08x\n", result);
        }
        else
        {
            if (L)
            {
                //word loaded from memory
                registers[Rd] = to_register(result, memory);
            }
            else
            {
                //word stored into memory
                to_memory(registers[Rd], memory, result);
            }
        }
    }
    else
    {
        if (result >= (1 << 16))
        {
            printf("Error: Out of bounds memory access at address 0x%08x\n", result);
        }
        else
        {
            if (L)
            {
                //word loaded from memory
                registers[Rd] = to_register(registers[Rn], memory);
            }
            else
            {
                //word stored into memory
                to_memory(registers[Rd], memory, registers[Rn]);
            }
            //offset is added/subtracted to base register after transferring data
            //change contents of base register by offset
            registers[Rn] += U ? interp_offset : -interp_offset;
        }
    }
}

void execute_branch(uint32_t *registers, uint32_t instr)
{
    int32_t offset = instr & mask(24);

    /*
        This offset is shifted left 2 bits, sign extended to 32 bits and then added to the PC.
        The offset will take into account the effect of the pipeline (i.e. PC is 8 bytes ahead of the
        instruction that is being executed).
      */

    offset = offset << 2;
    int firstBit = offset >> 25;
    if (firstBit)
    {
        offset = offset | (mask(6) << 26);
    }
    registers[PC] = ((int32_t)registers[PC]) + offset;
}