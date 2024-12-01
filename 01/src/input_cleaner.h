#ifndef INPUT_CLEANER
#define INPUT_CLEANER

#include <stdlib.h>

typedef struct ArrayTupple {
	long* l1;
	long* l2;

	size_t head;
	size_t max_length;
	int is_constructed;
} ArrayTupple;

ArrayTupple get_arrays_from_input();
void destroy_tupple(ArrayTupple* target);

#endif