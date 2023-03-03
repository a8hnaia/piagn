#include "etc.h"

void help() {
	puts(
	"piagn: missing file name\n"
	"piagn <file name>"
	);
}

int main(int argc, char* argv[]) {
	if (argc < 2)
	{
		help();
		return 1;
	}

	if (!parse_file(argv[1]))
	{
		printf("Error parsing file: %s\n", strerror(errno));
		return 1;
	}
	
	struct elem_t_dynarr stack = DYNARR_NEW(elem_t, 16);
	DYNARR_ASSERT(stack);
	exec_function(&stack, funcs[0]);
	
	DYNARR_FREE(stack);
}
