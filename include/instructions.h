#ifndef PIAGN_INSTR_H
#define PIAGN_INSTR_H

#define STACK_POP(DYN) DYN.len ? DYNARR_POP(DYN) : 0

#include "common.h"

void op_nop(state_t* s); // null operation
void op_1(state_t* s); // 1
void op_2(state_t* s); // 2
void op_3(state_t* s); // 3
void op_4(state_t* s); // 4
void op_5(state_t* s); // 5
void op_6(state_t* s); // 6
void op_7(state_t* s); // 7
void op_8(state_t* s); // 8
void op_9(state_t* s); // 9
void op_0(state_t* s); // 0
void op_rep(state_t* s); // #
void op_rand(state_t* s); // ?
void op_dup(state_t* s); // !
void op_swap(state_t* s); // %
void op_pop(state_t* s); // _
void op_xor(state_t* s); // ^
void op_or(state_t* s); // +
void op_and(state_t* s); // &
void op_lsl(state_t* s); // [
void op_lsr(state_t* s); // ]
void op_not(state_t* s); // ~
void op_go(state_t* s); // $
void op_pl(state_t* s); // <
void op_pr(state_t* s); // >
void op_pts(state_t* s); // ,
void op_stp(state_t* s); // .
void op_in(state_t* s); // ;
void op_out(state_t* s); // :
void op_mr(state_t* s); // / forward slash
void op_ml(state_t* s); // \ backslash
void op_flow(state_t* s, int vt); // generalisation of - and |
void op_hz(state_t* s); // -
void op_vt(state_t* s); // |
void op_st(state_t* s); // *

extern opptr_t ops[128];

#endif
