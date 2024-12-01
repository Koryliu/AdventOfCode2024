#ifndef INPUT_CLEANER
#define INPUT_CLEANER

#include <stdlib.h>

typedef struct ArrayTupple {
	long* left;
	long* right;

	size_t length;
	size_t max_length;
	int is_allocated;
} ArrayTupple;

ArrayTupple get_arrays_from_input();
ArrayTupple create_tupple();
void destroy_tupple(ArrayTupple* target);

#endif