#include "etc.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr,
		"piagn: missing file name\n"
		"piagn <file name>"
		);
		return 1;
	}

	int error = parse_file(argv[1]);
	if(error) {
		static const char* error_texts[] = {
			"Errno",
			"Multiple `@`s",
			"Multiple operations around `@`",
			"No operation around `@`",
		};

		const char* text = (error == 1) ? strerror(errno)
										: error_texts[error];
		fprintf(stderr, "Error parsing file: %s\n", text);

		return error;
	}
	
	struct elem_t_dynarr stack = DYNARR_NEW(elem_t, 16);
	DYNARR_ASSERT(stack);
	exec_function(&stack, funcs[0]);

	DYNARR_FREE(stack);
}
