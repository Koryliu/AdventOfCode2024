#include "input_cleaner.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int get_input(char** destination) {
	FILE* input = fopen("input.txt", "r");
	if ((input == NULL)) {
		fprintf(stderr, "Error! Couldn't open given input file!");
		return 0;
	}
	fseek(input, 0, SEEK_END);
	long input_size = ftell(input) + 1;
	fseek(input, 0, SEEK_SET);  // wil only work for disk files!

	*destination = malloc(input_size * sizeof(char));
	char* offset = *destination;
	while ((offset = fgets(offset, input_size, input)) != NULL) {
		offset += strlen(offset);
	}
	fclose(input);

	if (*destination == NULL) {
		free(*destination);
		fprintf(stderr, "Error! Encountered an error during reading of file contents");
		return 0;
	}
	printf("%s", *destination);
	return 1;
}
