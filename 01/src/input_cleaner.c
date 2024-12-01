#include "input_cleaner.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ALLOCATION_SEGMENT 200


static void sorting_insert(long* target, const size_t length, const long val) {
	size_t insert_to = 0;
	for (;insert_to < length; insert_to++) {
		if (target[insert_to] > val) {
			break;
		}
	}

	size_t bytes_moved = (length - insert_to) * sizeof(long);
	if (bytes_moved > 0) {
		long* temp_array = malloc(bytes_moved);
		memcpy(temp_array, target + insert_to, bytes_moved);
		memcpy(target + insert_to + 1, temp_array, bytes_moved);
		free(temp_array);
	}
	target[insert_to] = val;
}


ArrayTupple get_arrays_from_input() {
	ArrayTupple result = create_tupple();
	if (!result.is_allocated) {
		return result;
	}

	FILE* input = fopen("input.txt", "r");
	if ((input == NULL)) {
		fprintf(stderr, "Error! Couldn't open given input file!");
		destroy_tupple(&result);
		return result;
	}

	size_t buffer_len = 20;
	char* read_buffer = malloc(buffer_len * sizeof(char));
	while (getline(&read_buffer, &buffer_len, input) > 0) {
		char* split = strpbrk(read_buffer, " \t\v\f");
		long val1 = strtol(read_buffer, &split, 10);
		long val2 = strtol(split + 1, NULL, 10);

		if (result.length >= result.max_length) {
			result.max_length += ALLOCATION_SEGMENT;
			result.left = realloc(result.left, result.max_length * sizeof(long));
			result.right = realloc(result.right, result.max_length * sizeof(long));
			if ((result.left == NULL) || (result.right == NULL)) {
				fprintf(stderr, "Error! Memory could not be re-allocated properly!");
				destroy_tupple(&result);
				break;
			}
		}
		sorting_insert(result.left, result.length, val1);
		sorting_insert(result.right, result.length, val2);
		result.length++;
	}
	fclose(input);
	free(read_buffer);
	return result;
}


ArrayTupple create_tupple() {
	ArrayTupple result = {
		.left  = malloc(ALLOCATION_SEGMENT * sizeof(long)),
		.right = malloc(ALLOCATION_SEGMENT * sizeof(long)),
		.length = 0,
		.max_length = ALLOCATION_SEGMENT,
		.is_allocated = true
	};
	if ((result.left == NULL) || (result.right == NULL)) {
		fprintf(stderr, "Error! Memory could not be allocated properly!");
		destroy_tupple(&result);
		return result;
	}
	return result;
}


void destroy_tupple(ArrayTupple* target) {
	free(target->left);
	target->left = NULL;
	free(target->right);
	target->right = NULL;

	target->length = 0;
	target->max_length = 0;
	target->is_allocated = false;
}
