#ifndef PIAGN_ETC_H
#define PIAGN_ETC_H

#include "instructions.h"

extern func_t funcs[64];

void exec_function(struct elem_t_dynarr* stack, func_t func);

int grid_at(grid_t grid, int x, int y);

void go_dir(enum direction dir, int* x, int* y);

int check_dir(grid_t grid, enum direction dir, int x, int y);

void surroundings(state_t* s, grid_t grid);

void step(state_t* s, grid_t grid);

int parse_file(const char const* file_name);

extern grid_t make_grid(char* str, int rows, int cols);

#endif
