#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "input_cleaner.h"


static long* binary_search(
	long* const search_in,
	const size_t max_length,
	const long value,
	const bool first_occurence)
{
	size_t low = 0;
    size_t high = max_length;
    long* search_target = NULL;

    while (low <= high) {
        size_t mid = (low + high) / 2;

        if (search_in[mid] > value) {
            high = mid - 1;
		}
        else if (search_in[mid] < value) {
            low = mid + 1;
		}
        else if (first_occurence) {
            high = mid - 1;
            search_target = search_in + mid;
		}
        else {
            low = mid + 1;
            search_target = search_in + mid;
		}
	}

    return search_target;
}


int main() {
	ArrayTupple locations = get_arrays_from_input();
	if (!locations.is_constructed) {
		return EXIT_FAILURE;
	}

	unsigned long long total_distance = 0;
	long long similarity_score = 0;
	for (size_t i = 0; i < locations.head; i++) {
		total_distance += llabs((long long) (locations.l1[i] - locations.l2[i]));

		long* first_occurence = binary_search(
			locations.l2, locations.max_length, locations.l1[i], true);
		if (first_occurence != NULL) {
			long* last_occurence = binary_search(
				locations.l2, locations.max_length, locations.l1[i], false);
			if (last_occurence == NULL) {
				fprintf(stderr, "Error! Binary search found beginning but not the end!");
				destroy_tupple(&locations);
				return EXIT_FAILURE;
			}
			similarity_score += locations.l1[i] * (last_occurence - first_occurence + 1);
		}
	}
	printf("Total score: %llu\n", total_distance);
	printf("Similarity score: %lli\n", similarity_score);

	destroy_tupple(&locations);
	return EXIT_SUCCESS;
}
