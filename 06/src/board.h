#ifndef BOARD
#define BOARD


typedef enum PositionState_e {
	NORTH         = 0b00000001,
	EAST          = 0b00000010,
	SOUTH         = 0b00000100,
	WEST          = 0b00001000,
	EMPTY         = 0b00010000,
	BLOCK         = 0b00100000,
	OUT_OF_BOUNDS = 0b01000000
} PositionState;
typedef enum PositionState_e Heading;

typedef struct Position_s {
	int x;
	int y;
} Position;

typedef struct Board_s {
	unsigned char** rows;
	int rows_count;
	Position p;
} Board;

Board get_board(const char* file_name);
void destruct_board(Board board);
void draw_board(Board board);

PositionState get_pos(Board board, Position pos);
Heading next_heading(Heading heading);
void add_hpos(Position* pos, const Heading heading);

#endif
