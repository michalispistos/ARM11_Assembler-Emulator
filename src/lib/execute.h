#ifndef EXECUTE_H
#define EXECUTE_H
//int checkCondition(uint32_t instr, uint32_t *registers);

//void executeDataProcess(int *registers, uint32_t instr);

//void executeMultiply(int *registers, uint32_t instr);

//void executeSingleDataTransfer(int *registers, uint32_t* memory, uint32_t instr);

//void executeBranch(int *registers, uint32_t instr);

void execute(uint32_t decoded, uint32_t instr, uint32_t *registers, uint32_t* memory);
#endif