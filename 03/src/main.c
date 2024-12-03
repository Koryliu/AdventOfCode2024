#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "input_cleaner.h"


static char* find_next_segment(const char* from, const bool is_dont) {
	if (from == NULL) {
		return NULL;
	}

	if (is_dont) {
		return strstr(from, "do()");
	}
	return strstr(from, "don't()");
}


int main() {
	char* string;
	if (!get_input(&string)) {
		return EXIT_FAILURE;
	}
	const char* string_pos = string;
	const char* segment_end = find_next_segment(string_pos, false);

	int val1, val2;
	char closer;

	long total = 0;
	size_t amounts = 0;
	while ((string_pos = strstr(string_pos, "mul(")) != NULL) {
		string_pos += strlen("mul(");
		if ((segment_end != NULL) && (string_pos - segment_end) >= 0) {
			string_pos = find_next_segment(string_pos, true);
			segment_end = find_next_segment(string_pos, false);
		}
		else if ((sscanf(string_pos, "%3d,%3d%c", &val1, &val2, &closer) == 3) && (closer == ')')) {
			total += val1 * val2;
			amounts++;
			string_pos += strlen("x,y)");
		}
	}

	free(string);
	printf("Totaled multiplication: %ld\n", total);
	printf("Amounts: %ld\n", amounts);
	return EXIT_SUCCESS;
}
