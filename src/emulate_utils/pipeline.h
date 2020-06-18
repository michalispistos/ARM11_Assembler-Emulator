#ifndef PIPELINE_H
#define PIPELINE_H

//fetches next instruction from memory according to PC value
uint32_t fetch(uint32_t *registers, uint32_t *memory);

// decodes an instruction and returns the enum corresponding to the type of instruction
uint32_t decode(uint32_t instr);

//calls the appropriate execute function according to the type of instruction given.
int execute(uint32_t decoded, uint32_t instr, uint32_t *registers, uint32_t *memory);

#endif