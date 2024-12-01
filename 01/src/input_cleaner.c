#include "input_cleaner.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ALLOCATION_SEGMENT 200


static void sorting_insert(long* target, const size_t head, const long val) {
	size_t insert_to = 0;
	for (;insert_to < head; insert_to++) {
		if (target[insert_to] > val) {
			break;
		}
	}

	long val_to_move = val;
	for (size_t i = insert_to; i <= head; i++) {
		long temp = target[i];
		target[i] = val_to_move;
		val_to_move = temp;
	}
}


ArrayTupple get_arrays_from_input() {
	ArrayTupple result = {
		malloc(ALLOCATION_SEGMENT * sizeof(long)),
		malloc(ALLOCATION_SEGMENT * sizeof(long)),
		0, ALLOCATION_SEGMENT, true
	};
	if ((result.l1 == NULL) || (result.l2 == NULL)) {
		fprintf(stderr, "Error! Memory could not be allocated properly!");
		destroy_tupple(&result);
		return result;
	}

	FILE* input = fopen("input.txt", "r");
	if ((input == NULL)) {
		fprintf(stderr, "Error! Couldn't open given input file!");
		destroy_tupple(&result);
		return result;
	}

	size_t buffer_len = 20;
	char* read_buffer = malloc(buffer_len * sizeof(char));  // will get allocated during getline!
	while (getline(&read_buffer, &buffer_len, input) > 0) {
		char* split = strpbrk(read_buffer, " \t\v\f");
		long val1 = strtol(read_buffer, &split, 10);
		long val2 = strtol(split + 1, NULL, 10);

		if (result.head == result.max_length) {
			result.max_length += ALLOCATION_SEGMENT;
			result.l1 = realloc(result.l1, result.max_length * sizeof(long));
			result.l2 = realloc(result.l2, result.max_length * sizeof(long));
			if ((result.l1 == NULL) || (result.l2 == NULL)) {
				fprintf(stderr, "Error! Memory could not be re-allocated properly!");
				destroy_tupple(&result);
				break;
			}
		}
		sorting_insert(result.l1, result.head, val1);
		sorting_insert(result.l2, result.head, val2);
		result.head++;
	}
	free(read_buffer);
	return result;
}


void destroy_tupple(ArrayTupple* target) {
	free(target->l1);
	target->l1 = NULL;
	free(target->l2);
	target->l2 = NULL;

	target->head = 0;
	target->max_length = 0;
	target->is_constructed = false;
}
