#ifndef PIAGN_INSTR_H
#define PIAGN_INSTR_H

#define STACK_POP(DYN) DYN.len ? DYNARR_POP(DYN) : 0

#include "common.h"

int exec_instruction(char op, state_t* s);

#endif
