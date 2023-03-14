#include "etc.h"

func_t funcs[64] = {0};

// Execute a custom defined function
void exec_function(struct elem_t_dynarr* stack, func_t func) {
	// Initialise the state struct
	state_t state = {
		.stack = *stack,
		.pile_i = 0,
		.repeat = -1,
		.dir = func.dir,
		.halt = 0,
		.x = func.x,
		.y = func.y
	};

	// If the function doesn't push to piles, don't allocate piles for it
	if (func.has_piles) {
		state.piles = malloc(sizeof(struct elem_t_dynarr) * 256);
		for (int i = 0; i < 256; i++) {
			state.piles[i] = DYNARR_NEW(elem_t, 2);
			DYNARR_ASSERT(state.piles[i]);
		}
	}
	else state.piles = NULL;

	// Step loop
	while (!state.halt) {
		step(&state, func.grid);
	}

	// Set parent's stack to the function's (for len and cap)
	*stack = state.stack;

	// Free if we allocated piles
	if (func.has_piles) {
		for (int i = 0; i < 256; i++) {
			DYNARR_FREE(state.piles[i]);
		}
		free(state.piles);
		state.piles = NULL;
	}
}

// Accessing grid position with bound checking
int grid_at(grid_t grid, int x, int y) {
	if (x > grid.cols || y > grid.rows ||
		x < 0   || y < 0) return -1;
	else return grid.ptr[y * grid.cols + x];
}

// Change x and y appropriately in the direction
void go_dir(enum direction dir, int* x, int* y) {
	switch (dir) {
		case UP   : --*y; break;
		case RIGHT: ++*x; break;
		case DOWN : ++*y; break;
		case LEFT : --*x; break;
	}
}

// Access the element in the direction without changing given x and y
int check_dir(grid_t grid, enum direction dir, int x, int y) {
	go_dir(dir, &x, &y);
	return grid_at(grid, x, y);
}

// Check the surroundings of the current position in the grid for `-` and `|`
void surroundings(state_t* s, grid_t grid) {
	int cw  = check_dir(grid, (s->dir + 1) & 3, s->x, s->y);
	int ccw = check_dir(grid, (s->dir - 1) & 3, s->x, s->y);

	// Halting edges count as empty
	if (cw == -1) cw = 0;
	if (ccw == -1) ccw = 0;

	if (cw && ccw) s->surrounding = 2;
	else if (cw) s->surrounding = 1;
	else if (ccw) s->surrounding = -1;
	else s->surrounding = 0;
}

// Execute given operation whether built-in or function
void exec_op(state_t* s, int op) {
	if (('A' <= op && 'Z' >= op) ||
		('a' <= op && 'z' >= op))
		exec_function(&s->stack, funcs[op - 64]);
	else exec_instruction(op, s);
}

// Take one step through the program
void step(state_t* s, grid_t grid) {
	surroundings(s, grid);
	int op = grid_at(grid, s->x, s->y);
	if (op == -1) {
		s->halt = 1;
		return;
	}
	if (s->repeat == -1)
		exec_op(s, op);
	else {
		for (int i = s->repeat; i > 0; i--) {
			exec_op(s, op);
		}
		s->repeat = -1;
	}
	go_dir(s->dir, &s->x, &s->y);
}

// Error code for make_func
int make_func_err = 0;
// Make a function from arguments
func_t make_func(char* str, int rows, int cols, int has_piles, int start_x, int start_y) {
	// Initialise grid struct
	grid_t grid = {
		.ptr = malloc(sizeof(char) * rows * cols),
		.rows = rows,
		.cols = cols
	};
	assert(grid.ptr);
	memset(grid.ptr, 0, sizeof(char) * rows * cols);

	// Iterate through the string
	int i = 0;
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols + 1; x++) {
			char a = str[i++];
			if (a == '\n') break;
			if (a == ' ') a = '\0';
			grid.ptr[y * cols + x] = a;
		}
	}

	// Figure out the way to start
	int dir;
	if (start_x == -1 && start_y == -1) {
		dir = RIGHT;
		start_x = 0;
		start_y = 0;
	}
	else {
		dir = 5;
		for (int i = 0; i < 4; i++) {
			char c = check_dir(grid, i, start_x, start_y);
			if (c == -1) c = 0;
			if (c) {
				if (dir == 5) dir = i;
				else {
					make_func_err = 3;
					return (func_t){};
				}
			}
		}
		if (dir == 5) {
			make_func_err = 4;
			return (func_t){};
		}
	}

	func_t func = (func_t){
		.grid = grid,
		.dir = dir,
		.has_piles = has_piles,
		.x = start_x,
		.y = start_y
	};
	return func;
}

// Parse file with the given file
int parse_file(const char const* file_name) {
	// Open the file
	FILE* fp = fopen(file_name, "r");
	if(!fp) return 1;

	// Make a buffer for the entire file
	struct char_dynarr buffer = DYNARR_NEW(char, 32);
	// Row and column count for the grid
	int rows = 0, cols = 0, cols_current = 0;
	// The position the function will start from
	int start_x = -1, start_y = -1;
	// Which function currently being parsed
	int func_id = 0;
	enum {
		OUTSIDE_FUNC = 0, // parsing main or not parsing a function
		START_OF_LINE, //start of line, might be the start of a function definition
		POSSIBLE_FUNCTION, // might be be about to parse function (to ignore spaces before opening brace)
		BEGIN_PARSING_FUNCTION, // will parse function after newline (to ignore characters after opening brace)
		PARSING_FUNCTION,
	} status = START_OF_LINE;
	// Whether current function ever pushes to a pile
	int has_piles = 0;
	// Whether the file only has a main function (without braces)
	int only_main = 1;

	// Until file ends
	while (!feof(fp)) {
		char c = fgetc(fp);
		if (status == START_OF_LINE) {
			if (('A' <= c && 'Z' >= c) ||
				('a' <= c && 'z' >= c)) {
				func_id = c - 64;
				status = POSSIBLE_FUNCTION;
				continue;
			}
			else if (c == '{') {
				func_id = 0;
				status = BEGIN_PARSING_FUNCTION;
				continue;
			}
			else status = OUTSIDE_FUNC;
			// No continue, should fall back to 0
		}
		if (status == POSSIBLE_FUNCTION) {
			if (c == '{') {
				status = BEGIN_PARSING_FUNCTION;
				continue;
			}
			else if (c != ' ') status = OUTSIDE_FUNC;
			// No continue, should fall back to 0
		}
		if (status == BEGIN_PARSING_FUNCTION && c == '\n') {
			DYNARR_RESIZE(buffer, 32);
			buffer.len = 0;
			rows = 0;
			cols = 0;
			cols_current = 0;
			start_x = -1;
			start_y = -1;
			has_piles = 0;
			only_main = 0;
			status = PARSING_FUNCTION;
			continue;
		}
		if ((status == OUTSIDE_FUNC && only_main) ||
			(status == POSSIBLE_FUNCTION && only_main) ||
			 status == PARSING_FUNCTION) {
			if (cols_current == 0 && status == PARSING_FUNCTION && c == '}') {
				funcs[func_id] = make_func(buffer.ptr, rows, cols, has_piles, start_x, start_y);
				if (make_func_err) return make_func_err;
				status = OUTSIDE_FUNC;
				continue;
			}
			// Enable piles if we push to a pile
			if (c == '.') has_piles = 1;
			if (c == '@') {
				if (start_x == -1 && start_y == -1) {
					start_x = cols_current;
					start_y = rows;
				}
				else return 2;
			}
			else DYNARR_PUSH(buffer, c);
			if (c == '\n') {
				rows++;
				if (cols_current > cols) cols = cols_current;
				cols_current = 0;
			}
			else cols_current++;
		}
		if (status == OUTSIDE_FUNC && c == '\n') status = START_OF_LINE;
	}
	fclose(fp);

	if (only_main)
		funcs[func_id] = make_func(buffer.ptr, rows + 1, cols, has_piles, start_x, start_y);
	if (make_func_err) return make_func_err;

	DYNARR_FREE(buffer);
	return 0;
}
