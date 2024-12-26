#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static TileValue transform_character(const unsigned char c) {
	switch(c) {

	case '.':
		return EMPTY;
	case '#':
		return BLOCK;
	case '^':
		return NORTH;
	case '>':
		return EAST;
	case 'V':
		return SOUTH;
	case '<':
		return WEST;
	default:
		fprintf(stderr, "Warning! Found unknown character, defaulting to empty space.\n");
		return EMPTY;
	}
}


Board get_board(const char* file_name) {
	Board board = {.rows = NULL,
				   .rows_count = -1,
				   .start = {.x = -1, .y = -1},
				   .p = {.x = -1, .y = -1},
				   .h = NORTH
	};
	FILE* input = fopen(file_name, "r");
	if (input == NULL) {
			fprintf(stderr, "Error! Couldn't open given input file!\n");
			return board;
	}

	int max_rows = 400;
	board.rows = malloc(max_rows * sizeof(char*));
	if (board.rows == NULL) {
		fprintf(stderr, "Error! Couldn't allocate memory for input!\n");
		return board;
	}

	int y = 0;
	while (1) {
		int x = 0;
		int max_cols = 200;
		unsigned char* line = calloc(max_cols, sizeof(unsigned char));
		while (1) {
			unsigned char read = fgetc(input);
			if (read == (unsigned char) EOF) {
				free(line);
				goto closer;
			}
			if (read == '\n') {
				break;
			}
			read = (unsigned char) transform_character(read);
			if ((read & (NORTH | EAST | SOUTH | WEST)) != 0x00000000) {
				board.start.x = x;
				board.start.y = y;
				board.p.x = x;
				board.p.y = y;
				board.h = read;
			}

			if (x == max_cols) {
				line = realloc(line, max_cols * 2 * sizeof(unsigned char));
				memset(line + max_cols, '\0', max_cols * sizeof(unsigned char));
				max_cols *= 2;
			}
			line[x++] = read;
		}

		if (y == max_rows) {
			max_rows *= 2;
			board.rows = realloc(board.rows, max_rows * sizeof(unsigned char*));
		}
		board.rows[y++] = line;
	}
	closer:
	board.rows_count = y;
	fclose(input);

	return board;
}


Board copy_board(Board* board) {
	Board copied = {.rows = malloc(board->rows_count * sizeof(unsigned char*)),
					.rows_count = board->rows_count,
					.start = board->start,
					.p = board->p,
					.h = board->h
	};

	for (size_t y = 0; y < board->rows_count; y++) {
		unsigned char* line = malloc((strlen(board->rows[y]) + 1) * sizeof(unsigned char));
		strcpy(line, board->rows[y]);
		copied.rows[y] = line;
	}
	return copied;
}


void destruct_board(Board* board) {
	if (board->rows != NULL) {
		for (int i = 0; i < board->rows_count; i++) {
			free(board->rows[i]);
			board->rows[i] = NULL;
		}
		free(board->rows);
		board->rows = NULL;
	}
	board->start.x = -1;
	board->start.y = -1;
	board->rows_count = -1;
	board->p.x = -1;
	board->p.y = -1;
	board->h = -1;
}


void draw_board(Board* board) {
	for (size_t y = 0; y < board->rows_count; y++) {
		for (size_t x = 0; x < strlen(board->rows[y]); x++) {
			char c = board->rows[y][x];
			if ((x == board->start.x) && (y == board->start.y)) {
				printf("^");
				continue;
			}

			if ((x == board->p.x) && (y == board->p.y)) {
				printf("o");
				continue;
			}

			switch (c) {

			case EMPTY:
				printf(".");
				break;
			case BLOCK:
				printf("#");
				break;
			default:
				if (((c & (NORTH | SOUTH)) != 0b00000000) && ((c & (EAST | WEST)) == 0b00000000)) {
					printf("|");
				} else
				if (((c & (NORTH | SOUTH)) == 0b00000000) && ((c & (EAST | WEST)) != 0b00000000)) {
					printf("-");
				} else {
					printf("+");
				}
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
}


static int find_next_heading(Board* board, Position* curr_p, Position* next_p, Heading* h) {
	for (size_t i = 0; (i < 5) && (get_pos(board, *next_p) == BLOCK); i++) {
		if (i == 4) {
			return 0;
		}
		*h = next_heading(board->h);
		*next_p = *curr_p;
		add_hpos(next_p, *h);
	}
	return 1;
}


unsigned int create_path(Board* board) {
	if ((board->p.x < 0) || (board->p.y < 0) || (board->rows == NULL)) {
		return 0;
	}

	// initial values
	Position next_p = board->p;
	add_hpos(&next_p, board->h);
	unsigned int tiles_visited = 1;

	if (!find_next_heading(board, &board->p, &next_p, &board->h)) {
		return tiles_visited;  // is stuck
	}

	while ((get_pos(board, next_p) & (board->h | OUT_OF_BOUNDS)) == 0) {
		tiles_visited += (get_pos(board, next_p) == EMPTY) ? 1 : 0;
		if (get_pos(board, next_p) != OUT_OF_BOUNDS) {
			board->rows[next_p.y][next_p.x] |= board->h;
			board->rows[board->p.y][board->p.x] |= board->h;
		}
		add_hpos(&board->p, board->h);
		add_hpos(&next_p, board->h);

		if (!find_next_heading(board, &board->p, &next_p, &board->h)) {
			return tiles_visited;  // is stuck
		}
	}
	return tiles_visited;
}


unsigned int create_path_blockades(Board* board) {
	if ((board->p.x < 0) || (board->p.y < 0) || (board->rows == NULL)) {
		return 0;
	}

	// initial values
	Position next_p = board->p;
	add_hpos(&next_p, board->h);
	unsigned int permutations = 0;

	if (!find_next_heading(board, &board->p, &next_p, &board->h)) {
		return permutations;  // is stuck
	}

	while ((get_pos(board, next_p) & (board->h | OUT_OF_BOUNDS)) == 0b00000000) {
		if (get_pos(board, next_p) != OUT_OF_BOUNDS) {
			Board permutation = copy_board(board);
			permutation.rows[next_p.y][next_p.x] = BLOCK;
			create_path(&permutation);
			if (is_path_loop(&permutation)) {
				draw_board(&permutation);
				permutations++;
			}
			destruct_board(&permutation);

			board->rows[next_p.y][next_p.x] |= board->h;
			board->rows[board->p.y][board->p.x] |= board->h;
		}
		add_hpos(&board->p, board->h);
		add_hpos(&next_p, board->h);

		if (!find_next_heading(board, &board->p, &next_p, &board->h)) {
			return permutations;  // is stuck
		}
	}
	return permutations;
}


int is_path_loop(Board* board) {
	// Position curr_p = board->start;
	// Position next_p = curr_p;
	// Heading heading = NORTH;
	// add_hpos(&next_p, heading);
	// if (find_next_heading(board, &curr_p, &next_p, &heading)) {
	// 	return 1;
	// }

	Position final_position = board->p;
	add_hpos(&final_position, board->h);
	return get_pos(board, final_position) != OUT_OF_BOUNDS;
}


TileValue get_pos(Board* board, Position pos) {
	if (
		(pos.y < 0) || (pos.x < 0) ||
		(pos.y >= board->rows_count) || (pos.x >= strlen(board->rows[pos.y]))
	   ) {
		return OUT_OF_BOUNDS;
	}
	return board->rows[pos.y][pos.x];
}


Heading next_heading(Heading heading) {
	if ((heading != NORTH) && (heading != EAST) && (heading != SOUTH) && (heading != WEST)) {
		fprintf(stderr, "Warning! Given heading is not actually a heading!\n");
		return heading;
	}

	if (heading == WEST) {
		heading = NORTH;
	} else {
		heading <<= 1;
	}
	return heading;
}


void add_hpos(Position* pos, const Heading heading) {
	switch (heading) {

	case NORTH:
		pos->y -= 1;
		break;
	case EAST:
		pos->x += 1;
		break;
	case SOUTH:
		pos->y += 1;
		break;
	case WEST:
		pos->x -= 1;
		break;
	default:
		fprintf(stderr, "Warning! Given heading is not an applicable heading!\n");
		break;
	}
}
