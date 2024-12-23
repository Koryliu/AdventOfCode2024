#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"


static unsigned int navigate(Board b) {
	if ((b.p.x < 0) || (b.p.y < 0) || (b.rows == NULL)) {
		return 0;
	}

	// initial values
	PositionState heading = get_pos(b, b.p);
	Position next_p = b.p;
	add_hpos(&next_p, heading);
	int tiles_visited = 1;

	for (size_t i = 0; (i < 5) && (get_pos(b, next_p) == BLOCK); i++) {
		if (i == 4) {
			return tiles_visited;  // is stuck
		}
		heading = next_heading(heading);
		next_p = b.p;
		add_hpos(&next_p, heading);
	}

	while ((get_pos(b, next_p) & (heading | OUT_OF_BOUNDS)) == 0) {
		// draw_board(b);
		tiles_visited += (get_pos(b, next_p) == EMPTY) ? 1 : 0;
		if (get_pos(b, next_p) != OUT_OF_BOUNDS) {
			b.rows[next_p.y][next_p.x] |= heading;
		}
		add_hpos(&b.p, heading);
		add_hpos(&next_p, heading);

		for (size_t i = 0; (i < 5) && (get_pos(b, next_p) == BLOCK); i++) {
			if (i == 4) {
				return tiles_visited;  // is stuck
			}
			heading = next_heading(heading);
			next_p = b.p;
			add_hpos(&next_p, heading);
		}
	} ;
	return tiles_visited;
}


int main() {
	// example test
	{
		Board b = get_board("example.txt");
		// Position start = b.p;

		int tiles_visited = navigate(b);
		destruct_board(b);
		if (tiles_visited != 41) {
			fprintf(stderr, "Error! Guard visited %i tiles, ", tiles_visited);
			fprintf(stderr, "but was supposed to visit 41!\n");
			return EXIT_FAILURE;
		}
	}

	{
		Board b = get_board("input.txt");
		int tiles_visited = navigate(b);
		destruct_board(b);
		printf("Tiles visited %i.\n", tiles_visited);
	}
	return EXIT_SUCCESS;
}