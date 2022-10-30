#include <ncurses.h>
#include <stdio.h>
#include <string.h>

// The board's boundaries
#define WIDTH 7
#define HEIGHT 6

// The ascii board boundaries
#define ASCII_HEIGHT 7
#define ASCII_WIDTH 50


// Initializing the board
int board[HEIGHT][WIDTH] = {
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0}
};

/*
  The size of the string will be constant
  We will have 7 columns each will look like :
  | _ | = 5 chars
   7 * 5 = 35
   and since 6 of them will have a shared colum
*/
char asciiboard[ASCII_HEIGHT][ASCII_WIDTH];

/*
  drawboard() will store the ascii shape of the board
  in the asciiboard variable.
*/
void drawboard() {

  char emptytemp[] = "| _ ";
  char xtemp[] = "| X ";
  char otemp[] = "| O ";
  char rowend[] = "|\n";

  char asciirow[ASCII_WIDTH];

  for(int i = 0; i < HEIGHT; i++) {
    for(int j = 0; j < WIDTH; j++) {
      if (board[i][j] == 1) {
        strcat(asciirow, xtemp);
      } else if (board[i][j] == 2) {
        strcat(asciirow, otemp);
      } else {
        strcat(asciirow, emptytemp);
      }
    }
    strcat(asciirow, rowend);
    strcpy(asciiboard[i], asciirow);
    strcpy(asciirow, ""); // Reinitialize the row
  }
  char finalrow[] = "  1   2   3   4   5   6   7  \n";
  strcpy(asciiboard[ASCII_HEIGHT - 1], finalrow);
}

/*
  printboard() will print the board in the window
*/
void printboard(int row, int col) {
  int boardheight = (int)(sizeof(asciiboard) / sizeof(asciiboard[0]));
  int count = (int) - ((sizeof(asciiboard) / sizeof(asciiboard[0])) / 2);
  for(int i = 0; i < boardheight; i++) {
    mvprintw((row/ 2) + count, (col - 30)/2, "%s", asciiboard[i]);
    count++;
  }
}

int main() {

  // The number of rows and columns on the screen
  int row, col;

  drawboard();

  initscr();
  getmaxyx(stdscr, row, col);
  printboard(row, col);
  refresh();
  getch();
  endwin();


  return 0;
}
