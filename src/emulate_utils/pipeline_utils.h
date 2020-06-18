#ifndef PIPELINE_UTILS_H
#define PIPELINE_UTILS_H

#define PC (15)
#define CPSR (16)

//returns condition code from given instruction
uint32_t condition(uint32_t instr);

//checks if CPSR flags meet condition code of given instruction
int check_condition(uint32_t instr, uint32_t *registers);

//executes a data process instruction
void execute_data_process(uint32_t *registers, uint32_t instr);

//executes a multiply instruction
void execute_multiply(uint32_t *registers, uint32_t instr);

//executes a single data trasnfer instruction
void execute_single_data_transfer(uint32_t *registers, uint32_t *memory, uint32_t instr);

//executes a branch instruction
void execute_branch(uint32_t *registers, uint32_t instr);

#endif