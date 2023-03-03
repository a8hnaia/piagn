#define DYNARR_TYPE(T) struct T ## _dynarr { \
	T typeid; \
	size_t len; \
	size_t cap; \
	int resize_amount; \
	T* ptr; \
}

#define DYNARR_NEW(T, SIZE) (struct T ## _dynarr){ \
	.len = 0, \
	.cap = SIZE, \
	.ptr = malloc(sizeof(T) * SIZE), \
}

#define DYNARR_ASSERT(DYN) assert(DYN.ptr);

#define DYNARR_RESIZE(DYN, NEWSIZE) {\
	DYN.cap = NEWSIZE; \
	DYN.ptr = realloc(DYN.ptr, sizeof(DYN.typeid) * NEWSIZE); \
}

#define DYNARR_PUSH(DYN, ELEM) { \
	while (DYN.cap <= DYN.len) \
		DYNARR_RESIZE(DYN, DYN.cap * 2); \
	(DYN.ptr)[(DYN.len)++] = ELEM; \
}

#define DYNARR_POP(DYN) DYN.ptr[--DYN.len]

#define DYNARR_TOP(DYN) DYN.ptr[DYN.len - 1]

#define DYNARR_ADJUST(DYN) { \
	while (DYN.len <= DYN.cap / 2 && DYN.cap > 1) \
		DYNARR_RESIZE(DYN, DYN.cap / 2); \
}

#define DYNARR_AT(DYN, I) DYN.ptr[I]

#define DYNARR_FREE(DYN) free(DYN.ptr)
