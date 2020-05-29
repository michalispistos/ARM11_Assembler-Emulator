#ifndef PIPELINE_UTILS_H
#define PIPELINE_UTILS_H

uint32_t mask(int no_of_bits);
uint32_t condition(uint32_t instr);
int checkCondition(uint32_t instr, uint32_t *registers);
void executeDataProcess(uint32_t *registers, uint32_t instr);
void executeMultiply(uint32_t *registers, uint32_t instr);
void executeSingleDataTransfer(uint32_t *registers, uint32_t *memory, uint32_t instr);
void executeBranch(uint32_t *registers, uint32_t instr);

#endif