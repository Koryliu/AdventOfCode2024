#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static PositionState transform_character(const unsigned char c) {
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
	Board board = {.rows = NULL, .rows_count = -1, .p = {.x = -1, .y = -1}};
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
				board.p.x = x;
				board.p.y = y;
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


void destruct_board(Board board) {
	if (board.rows != NULL) {
		for (int i = 0; i < board.rows_count; i++) {
			free(board.rows[i]);
			board.rows[i] = NULL;
		}
		free(board.rows);
		board.rows = NULL;
	}
	board.rows_count = -1;
	board.p.x = -1;
	board.p.y = -1;
}


void draw_board(Board board) {
	for (size_t y = 0; y < board.rows_count; y++) {
		for (size_t x = 0; x < strlen(board.rows[y]); x++) {
			char c = board.rows[y][x];
			switch (c) {

			case EMPTY:
				printf(".");
				break;
			case BLOCK:
				printf("#");
				break;
			default:
				printf("X");
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
}


PositionState get_pos(Board board, Position pos) {
	if (
		(pos.y < 0) || (pos.x < 0) ||
		(pos.y >= board.rows_count) || (pos.x >= strlen(board.rows[pos.y]))
	   ) {
		return OUT_OF_BOUNDS;
	}
	return board.rows[pos.y][pos.x];
}


Heading next_heading(Heading heading) {
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
