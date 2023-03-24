#include "instructions.h"

void op_flow(state_t* s, int vt) { // generalisation of - and |
	int dir = s->dir;
	// rotate the `switch`ed direction if vertical
	if (vt) dir = (dir + 1) & 3;

	if(dir == UP || dir == DOWN) {
		switch (s->surrounding) {
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

int exec_instruction(char op, state_t* s) {
	switch (op) {
		case ' ': case '\0': break;

		case '0':
			DYNARR_PUSH(s->stack, 0);
			break;

		case '1' ... '8':
			DYNARR_PUSH(s->stack, (1 << (op - 48)-1));
			break;

		case '9':
			DYNARR_PUSH(s->stack, 255);
			break;

		case '#':
			s->repeat = STACK_POP(s->stack);
			break;

		case '?':
			DYNARR_PUSH(s->stack, rand() & 255);
			break;

		case '!': {
			int a = DYNARR_TOP(s->stack);
			DYNARR_PUSH(s->stack, a);
			break;
		}

		case '%': {
			elem_t a = STACK_POP(s->stack);
			elem_t b = STACK_POP(s->stack);
			DYNARR_PUSH(s->stack, a);
			DYNARR_PUSH(s->stack, b);
			break;
		}

		case '_':
			STACK_POP(s->stack);
			break;

		case '^': {
			elem_t a = STACK_POP(s->stack);
			elem_t b = STACK_POP(s->stack);
			DYNARR_PUSH(s->stack, a ^ b);
			break;
		}

		case '+': {
			elem_t a = STACK_POP(s->stack);
			elem_t b = STACK_POP(s->stack);
			DYNARR_PUSH(s->stack, a | b);
			break;
		}

		case '&': {
			elem_t a = STACK_POP(s->stack);
			elem_t b = STACK_POP(s->stack);
			DYNARR_PUSH(s->stack, a & b);
			break;
		}

		case '[': {
			elem_t a = STACK_POP(s->stack);
			DYNARR_PUSH(s->stack, a << 1);
			break;
		}

		case ']': {
			elem_t a = STACK_POP(s->stack);
			DYNARR_PUSH(s->stack, a >> 1);
			break;
		}

		case '~': {
			elem_t a = STACK_POP(s->stack);
			DYNARR_PUSH(s->stack, ~a);
			break;
		}

		case '$': {
			elem_t a = STACK_POP(s->stack);
			s->pile_i = a;
			break;
		}

		case '<':
			s->pile_i++;
			s->pile_i &= 255;
			break;

		case '>':
			s->pile_i--;
			s->pile_i &= 255;
			break;

		case ',': {
			if (!s->piles) {
				DYNARR_PUSH(s->stack, 0);
				break;
			}
			elem_t a = STACK_POP(s->piles[s->pile_i]);
			DYNARR_PUSH(s->stack, a);
			break;
		}

		case '.': {
			if (!s->piles) break;
			elem_t a = STACK_POP(s->stack);
			DYNARR_PUSH(s->piles[s->pile_i], a);
			break;
		}

		case ';':
			assert(!feof(stdin));
			DYNARR_PUSH(s->stack, fgetc(stdin));
			break;

		case ':': {
			elem_t a = STACK_POP(s->stack);
			fputc(a, stdout);
			break;
		}

		case '/':
			s->dir ^= 1;
			break;

		case '\\':
			s->dir ^= 3;
			break;

		case '-':
			op_flow(s, 0);
			break;

		case '|':
			op_flow(s, 1);
			break;

		case '*': {
			int a = STACK_POP(s->stack);
			// counter clockwise
			if      (a == 255) s->dir = (s->dir - 1) & 3;
			// clockwise
			else if (a == 0)   s->dir = (s->dir + 1) & 3;
			break;
		}

		default:
			return 1;
		break;
	}
	return 0;
}
