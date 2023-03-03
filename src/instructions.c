#include "instructions.h"

void op_nop(state_t* s) {
}

void op_1(state_t* s) { // 1
	DYNARR_PUSH(s->stack, 1);
}

void op_2(state_t* s) { // 2
	DYNARR_PUSH(s->stack, 2);
}

void op_3(state_t* s) { // 3
	DYNARR_PUSH(s->stack, 4);
}

void op_4(state_t* s) { // 4
	DYNARR_PUSH(s->stack, 8);
}

void op_5(state_t* s) { // 5
	DYNARR_PUSH(s->stack, 16);
}

void op_6(state_t* s) { // 6
	DYNARR_PUSH(s->stack, 32);
}

void op_7(state_t* s) { // 7
	DYNARR_PUSH(s->stack, 64);
}

void op_8(state_t* s) { // 8
	DYNARR_PUSH(s->stack, 128);
}

void op_9(state_t* s) { // 9
	DYNARR_PUSH(s->stack, 255);
}

void op_0(state_t* s) { // 0
	DYNARR_PUSH(s->stack, 0);
}

void op_rep(state_t* s) { // #
	s->repeat = STACK_POP(s->stack);
}

void op_rnd(state_t* s) { // ?
	DYNARR_PUSH(s->stack, rand() & 255);
}

void op_dup(state_t* s) { // !
	int a = DYNARR_TOP(s->stack);
	DYNARR_PUSH(s->stack, a);
}

void op_swp(state_t* s) { // %
	elem_t a = STACK_POP(s->stack);
	elem_t b = STACK_POP(s->stack);
	DYNARR_PUSH(s->stack, a);
	DYNARR_PUSH(s->stack, b);
}

void op_pop(state_t* s) { // _
	STACK_POP(s->stack);
}

void op_xor(state_t* s) { // ^
	elem_t a = STACK_POP(s->stack);
	elem_t b = STACK_POP(s->stack);
	DYNARR_PUSH(s->stack, a ^ b);
}

void op_or(state_t* s) { // +
	elem_t a = STACK_POP(s->stack);
	elem_t b = STACK_POP(s->stack);
	DYNARR_PUSH(s->stack, a | b);
}

void op_and(state_t* s) { // &
	elem_t a = STACK_POP(s->stack);
	elem_t b = STACK_POP(s->stack);
	DYNARR_PUSH(s->stack, a & b);
}

void op_lsl(state_t* s) { // [
	elem_t a = STACK_POP(s->stack);
	DYNARR_PUSH(s->stack, a << 1);
}

void op_lsr(state_t* s) { // ]
	elem_t a = STACK_POP(s->stack);
	DYNARR_PUSH(s->stack, a >> 1);
}

void op_not(state_t* s) { // ~
	elem_t a = STACK_POP(s->stack);
	DYNARR_PUSH(s->stack, ~a);
}

void op_go(state_t* s) { // $
	elem_t a = STACK_POP(s->stack);
	s->pile_i = a;
}

void op_pl(state_t* s) { // <
	s->pile_i++;
	s->pile_i &= 255;
}

void op_pr(state_t* s) { // >
	s->pile_i--;
	s->pile_i &= 255;
}

void op_pts(state_t* s) { // ,
	if (!s->piles) {
		DYNARR_PUSH(s->stack, 0);
		return;
	}
	elem_t a = STACK_POP(s->piles[s->pile_i]);
	DYNARR_PUSH(s->stack, a);
}

void op_stp(state_t* s) { // .
	if (!s->piles) return;
	elem_t a = STACK_POP(s->stack);
	DYNARR_PUSH(s->piles[s->pile_i], a);
}

void op_in(state_t* s) { // ;
	assert(!feof(stdin));
	DYNARR_PUSH(s->stack, fgetc(stdin));
}

void op_out(state_t* s) { // :
	elem_t a = STACK_POP(s->stack);
	fputc(a, stdout);
}

void op_mr(state_t* s) { // / forward slash
	switch (s->dir) {
		case UP   : s->dir = RIGHT; break;
		case RIGHT: s->dir = UP;    break;
		case DOWN : s->dir = LEFT;  break;
		case LEFT : s->dir = DOWN;  break;
	}
}

void op_ml(state_t* s) { // \ backslash
	switch (s->dir) {
		case UP   : s->dir = LEFT;  break;
		case RIGHT: s->dir = DOWN;  break;
		case DOWN : s->dir = RIGHT; break;
		case LEFT : s->dir = UP;    break;
	}
}

void op_flow(state_t* s, int vt) { // generalisation of - and |
	int dir = s->dir;
	// rotate the `switch`ed direction if vertical
	if (vt) dir = (dir + 1) & 3;
	switch (dir) {
		case LEFT: case RIGHT: break;
		case UP: case DOWN: switch (s->surrounding) {
			case 0: s->halt = 1; break;
			case 1: case -1:
				s->dir = (s->dir + s->surrounding) & 3;
			break;
			case 2: {
				elem_t a = STACK_POP(s->stack);
				int b = (!a) * 2 - 1;
				s->dir = (s->dir + b) & 3;
			}	break;
		}
	}
}

void op_hz(state_t* s) { // -
	op_flow(s, 0);
}

void op_vt(state_t* s) { // |
	op_flow(s, 1);
}

void op_st(state_t* s) { // *
	int a = STACK_POP(s->stack);
	// counter clockwise
	if      (a == 255) s->dir = (s->dir - 1) & 3;
	// clockwise
	else if (a == 0)   s->dir = (s->dir + 1) & 3;
}

opptr_t ops[] = {
	['\0'] = op_nop,
	[' '] = op_nop,
	['1'] = op_1,
	['2'] = op_2,
	['3'] = op_3,
	['4'] = op_4,
	['5'] = op_5,
	['6'] = op_6,
	['7'] = op_7,
	['8'] = op_8,
	['9'] = op_9,
	['0'] = op_0,
	['#'] = op_rep,
	['?'] = op_rnd,
	['!'] = op_dup,
	['%'] = op_swp,
	['_'] = op_pop,
	['^'] = op_xor,
	['+'] = op_or,
	['&'] = op_and,
	['['] = op_lsl,
	[']'] = op_lsr,
	['~'] = op_not,
	['$'] = op_go,
	['<'] = op_pl,
	['>'] = op_pr,
	[','] = op_pts,
	['.'] = op_stp,
	[';'] = op_in,
	[':'] = op_out,
	['/'] = op_mr,
	['\\'] = op_ml,
	['-'] = op_hz,
	['|'] = op_vt,
	['*'] = op_st
};
