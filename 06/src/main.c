#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"


int main() {
	// example test
	{
		Board b_template = get_board("example.txt");
		if (b_template.rows == NULL) {
			return EXIT_FAILURE;
		}
		Board b1 = copy_board(&b_template);
		Board b2 = copy_board(&b_template);
		destruct_board(&b_template);

		unsigned int tiles_visited = create_path(&b1);
		destruct_board(&b1);

		unsigned int possible_blockades = create_path_blockades(&b2);
		destruct_board(&b2);
		if (tiles_visited != 41) {
			fprintf(stderr, "Error! Guard visited %i tiles, ", tiles_visited);
			fprintf(stderr, "but was supposed to visit 41!\n");
			return EXIT_FAILURE;
		}
		if (possible_blockades != 6) {
			fprintf(stderr, "Error! Created %i blockades, ", possible_blockades);
			fprintf(stderr, "but was supposed to make 6!\n");
			return EXIT_FAILURE;
		}
	}

	{
		Board b_template = get_board("input.txt");
		if (b_template.rows == NULL) {
			return EXIT_FAILURE;
		}
		Board b1 = copy_board(&b_template);
		Board b2 = copy_board(&b_template);
		destruct_board(&b_template);

		unsigned int tiles_visited = create_path(&b1);
		destruct_board(&b1);

		unsigned int possible_blockades = create_path_blockades(&b2);
		destruct_board(&b2);
		printf("Tiles visited: %i.\n", tiles_visited);
		printf("Possible blockades: %i.\n", possible_blockades);
	}
	return EXIT_SUCCESS;
}