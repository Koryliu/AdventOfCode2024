#ifndef BOARD
#define BOARD


typedef enum TileValue_e {
	NORTH         = 0b00000001,  // heading
	EAST          = 0b00000010,  // heading
	SOUTH         = 0b00000100,  // heading
	WEST          = 0b00001000,  // heading
	EMPTY         = 0b00010000,
	BLOCK         = 0b00100000,
	OUT_OF_BOUNDS = 0b01000000
} TileValue;
// Heading should be special cases of TileValue where allowing only North, East, South or West
// Which should only ever be exclusively one of these four.
typedef enum TileValue_e Heading;

typedef struct Position_s {
	int x;
	int y;
} Position;

typedef struct Board_s {
	unsigned char** rows;
	int rows_count;
	Position start;
	Position p;
	Heading h;
} Board;

// Constructs board from data at file_name.
// Returned board is allocated and should be destructed once it's no longer needed.
Board get_board(const char* file_name);
// Creates a deep copy of a board.
// Returned board is allocated and should be destructed once it's no longer needed.
Board copy_board(Board* board);
// Destructs a board. If rows isn't a null pointer, deallocates it.
// Is also responsible for deallocating each pointer of rows.
// Therefore should not be used on boards which aren't dynamically allocated.
void destruct_board(Board* board);
// Draws the current board.
void draw_board(Board* board);

// Creates a path that the guard will take.
// Returns the total number of visited tiles.
// Tiles are counted only once, even if visited multiple times.
unsigned int create_path(Board* board);
// Creates a path that the guard will take.
// Returns the total number of blockades that will result in a loop.
unsigned int create_path_blockades(Board* board);
// Checks whether given path is a loop.
int is_path_loop(Board* board);

// Gets the tile value of a position on board.
TileValue get_pos(Board* board, Position pos);
// Gets the next heading. If given parameter isn't a heading,
// returns heading and prints a warning line to stderr.
Heading next_heading(Heading heading);
// Moves position 1 tile towards heading.
void add_hpos(Position* pos, const Heading heading);

#endif
