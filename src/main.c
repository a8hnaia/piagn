#include "etc.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr,
		"piagn: missing file name\n"
		"piagn <file name>"
		);
		return 1;
	}

	int result = parse_file(argv[1]);
	switch (result) {
		case 1:
			fprintf(stderr, "Error parsing file: %s\n", strerror(errno));
			return 1;
		break;
		case 2:
			fprintf(stderr, "Error parsing file: Multiple `@`s");
			return 2;
		break;
		case 3:
			fprintf(stderr, "Error parsing file: Multiple operations around `@`");
			return 3;
		break;
		case 4:
			fprintf(stderr, "Error parsing file: No operation around `@`");
			return 4;
		break;
	}
	
	struct elem_t_dynarr stack = DYNARR_NEW(elem_t, 16);
	DYNARR_ASSERT(stack);
	exec_function(&stack, funcs[0]);

	DYNARR_FREE(stack);
}
