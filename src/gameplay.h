#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

// The board's boundaries
#define WIDTH 7
#define HEIGHT 6

// The ascii board boundaries
#define ASCII_HEIGHT 7
#define ASCII_WIDTH 50

#define BIG_SPACE_R 4
#define SMALL_SPACE_R 2

// since col ≈ row * 2
#define BIG_SPACE_C 8
#define SMALL_SPACE_C 4

// extern - defined in gameplay.c
extern int board[HEIGHT][WIDTH];
extern char ascii_board[ASCII_HEIGHT][ASCII_WIDTH];

// prototype
int gameplay();

#endif
