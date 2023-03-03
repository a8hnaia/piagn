#include "etc.h"

func_t funcs[64];

void exec_function(struct elem_t_dynarr* stack, func_t func) {
	state_t state = {
		.stack = *stack,
		.pile_i = 0,
		.repeat = 1,
		.dir = RIGHT,
		.halt = 0,
		.x = 0,
		.y = 0
	};
	if (!func.no_piles) {
		state.piles = malloc(sizeof(struct elem_t_dynarr) * 256);
		for (int i = 0; i < 256; i++) {
			state.piles[i] = DYNARR_NEW(elem_t, 2);
			DYNARR_ASSERT(state.piles[i]);
		}
	}
	else state.piles = NULL;
	while (!state.halt) {
		step(&state, func.grid);
	}
	*stack = state.stack;
	if (!func.no_piles) {
		for (int i = 0; i < 256; i++) {
			DYNARR_FREE(state.piles[i]);
		}
	}
}

int grid_at(grid_t grid, int x, int y) {
	if (x > grid.cols || y > grid.rows ||
		x < 0   || y < 0) return -1;
	else return grid.ptr[y * grid.cols + x];
}

void go_dir(enum direction dir, int* x, int* y) {
	switch (dir) {
		case UP   : --*y; break;
		case RIGHT: ++*x; break;
		case DOWN : ++*y; break;
		case LEFT : --*x; break;
	}
}

int check_dir(grid_t grid, enum direction dir, int x, int y) {
	go_dir(dir, &x, &y);
	return grid_at(grid, x, y);
}

void surroundings(state_t* s, grid_t grid) {
	int cw  = check_dir(grid, (s->dir + 1) & 3, s->x, s->y);
	int ccw = check_dir(grid, (s->dir - 1) & 3, s->x, s->y);
	if (cw == -1) cw = 0;
	if (ccw == -1) ccw = 0;
	if (cw && ccw) s->surrounding = 2;
	else if (cw) s->surrounding = 1;
	else if (ccw) s->surrounding = -1;
	else s->surrounding = 0;
}

void step(state_t* s, grid_t grid) {
	surroundings(s, grid);
	int op = grid_at(grid, s->x, s->y);
	if (op == -1) {
		s->halt = 1;
		return;
	}
	if (('A' <= op && 'Z' >= op) ||
		('a' <= op && 'z' >= op)) {
		exec_function(&s->stack, funcs[op - 64]);
	}
	else (*ops[op])(s);
	go_dir(s->dir, &s->x, &s->y);
}

grid_t make_grid(char* str, int rows, int cols) {
	grid_t grid = {
		.ptr = malloc(sizeof(char) * rows * cols),
		.rows = rows,
		.cols = cols
	};
	assert(grid.ptr);
	memset(grid.ptr, 0, sizeof(char) * rows * cols);
	int i = 0;
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			char a = str[i++];
			if (a == '\n') break;
			if (a == ' ') a = '\0';
			grid.ptr[y * cols + x] = a;
		}
	}
	return grid;
}

int parse_file(const char const* file_name) {
	FILE* fp = fopen(file_name, "r");
	if(!fp) return 0;

	struct char_dynarr buffer = DYNARR_NEW(char, 32);
	int rows = 1, cols = 0, cols_current = 0;
	int only_main = 1;
	while (!feof(fp)) {
		char c = fgetc(fp);
		cols_current++;
		if (c == '\n') {
			rows++;
			if (cols_current > cols) {
				cols = cols_current;
				cols_current = 0;
			}
		}
		DYNARR_PUSH(buffer, c);
	}
	if (only_main) {
		funcs[0] = (func_t){
			.grid = make_grid(buffer.ptr, rows, cols),
			.dir = RIGHT,
			.no_piles = 0,
			.x = 0,
			.y = 0
		};
	}

	DYNARR_FREE(buffer);
	fclose(fp);
	return 1;
}
