#ifndef PIAGN_COMMON_H
#define PIAGN_COMMMN_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include "dynarr.h"

typedef uint8_t elem_t;

DYNARR_TYPE(elem_t);
DYNARR_TYPE(char);

enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

typedef struct {
	struct elem_t_dynarr stack;
	struct elem_t_dynarr* piles;
	int pile_i; // selected pile index
	int repeat;
	enum direction dir;
	int surrounding;
	int halt;
	int x;
	int y;
} state_t;

typedef struct {
	char* ptr;
	int rows;
	int cols;
} grid_t;

typedef struct {
	grid_t grid;
	enum direction dir;
	int no_piles;
	int x;
	int y;
} func_t;

#endif
