#ifndef PIAGN_COMMON_H
#define PIAGN_COMMMN_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dynarr.h"

#define elem_t uint8_t

DYNARR_TYPE(elem_t);
DYNARR_TYPE(char);

enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

struct state {
	struct elem_t_dynarr stack;
	struct elem_t_dynarr* piles;
	int pile_i; // selected pile index
	int repeat;
	enum direction dir;
	int surrounding;
	int halt;
	int x;
	int y;
};
typedef struct state state_t;

struct grid {
	char* ptr;
	int rows;
	int cols;
};
typedef struct grid grid_t;

struct func {
	grid_t grid;
	enum direction dir;
	int no_piles;
	int x;
	int y;
};
typedef struct func func_t;

typedef void (*opptr_t)(state_t*);

#endif
