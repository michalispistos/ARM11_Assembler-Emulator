#include <stdint.h>

void executeDataProcess(int *registers, uint32_t instr);

void executeMultiply(int *registers, uint32_t instr);

void executeSingleDataTransfer(int *registers, uint32_t instr);

void executeBranch(int *registers, uint32_t instr);