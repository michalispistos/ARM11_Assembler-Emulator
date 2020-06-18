#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemble_instr_utils.h"
#include "common_utils.h"

static uint32_t rotate_left(uint32_t word, int times)
{
    return (word << times) | (word >> (31 - times));
}

static int calculate_rotate_value(uint32_t word)
{
    for (int i = 0; i < 32; i += 2)
    {
        if (rotate_left(word, i) <= mask(8))
            return i;
    }
    perror("too large");
    exit(EXIT_FAILURE);
}

int calculate(uint32_t word)
{
    int rotateTimes = calculate_rotate_value(word);
    return ((rotateTimes / 2) << 8) | (word >> (32 - rotateTimes));
}

int get_value_from_token(char *registers)
{
    if (registers[0] == '#' || registers[0] == '=')
    {
        // <#expression>
        memmove(registers, registers + 1, strlen(registers));
        int res;
        if (registers[0] == '0' && registers[1] == 'x')
        {
            //hex
            res = strtol(registers, NULL, 16);
        }
        else if (registers[0] == '-' && registers[1] == '0' && registers[2] == 'x')
        {
            memmove(registers, registers + 1, strlen(registers));
            res = -1 * strtol(registers, NULL, 16);
        }
        else
        {
            //decimal
            res = strtol(registers, NULL, 10);
        }
        return res;
    }
    else if (registers[0] == '[')
    {
        // if there is a square bracket this will remove
        memmove(registers, registers + 1, strlen(registers));
    }
    //register
    memmove(registers, registers + 1, strlen(registers));
    int res = strtol(registers, NULL, 10);
    return res;
}

//by a constant
uint32_t shifted_register_expression(uint32_t word, char *shift_type, int shift_amount)
{
    int type;
    if (!strcmp("lsl", shift_type))
        type = 0;
    if (!strcmp("lsr", shift_type))
        type = 1;
    if (!strcmp("asr", shift_type))
        type = 2;
    if (!strcmp("ror", shift_type))
        type = 3;
    word |= (shift_amount << 7 | type << 5);
    return word;
}
// specified by register
uint32_t shifted_register_register(uint32_t word, char *shift_type, int Rs)
{
    int type;
    if (!strcmp("lsl", shift_type))
        type = 0;
    if (!strcmp("lsr", shift_type))
        type = 1;
    if (!strcmp("asr", shift_type))
        type = 2;
    if (!strcmp("ror", shift_type))
        type = 3;
    word |= (1 << 4 | Rs << 8 | type << 5);
    return word;
}

// used to remove ']' in token for sdt
int check_trailing_char(char *token, char character)
{
    if (token[strlen(token) - 1] == ']')
    { //pre index
        token[strcspn(token, "]")] = '\0';
        return 1;
    }
    return 0;
}