#ifndef PIPELINE_H
#define PIPELINE_H

uint32_t fetch(uint32_t *registers, uint32_t *memory);
uint32_t decode(uint32_t instr);
int execute(uint32_t decoded, uint32_t instr, uint32_t *registers, uint32_t* memory);

#endif