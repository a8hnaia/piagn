#include "etc.h"

int main(int argc, char* argv[]) {
	if (argc < 2) return 1;
	FILE* fp = fopen(argv[1], "r");
	parse_file(fp);
	fclose(fp);
	struct elem_t_dynarr stack = DYNARR_NEW(elem_t, 16);
	DYNARR_ASSERT(stack);
	exec_function(&stack, funcs[0]);
	DYNARR_FREE(stack);
}
