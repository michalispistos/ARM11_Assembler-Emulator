#ifndef PIPELINE_UTILS_H
#define PIPELINE_UTILS_H

#define PC (15)
#define CPSR (16)

uint32_t mask(int no_of_bits);
uint32_t condition(uint32_t instr);
int check_condition(uint32_t instr, uint32_t *registers);
void execute_data_process(uint32_t *registers, uint32_t instr);
void execute_multiply(uint32_t *registers, uint32_t instr);
void execute_single_data_transfer(uint32_t *registers, uint32_t *memory, uint32_t instr);
void execute_branch(uint32_t *registers, uint32_t instr);

#endif