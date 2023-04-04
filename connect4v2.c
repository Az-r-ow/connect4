#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "views.h"

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
  char rowend[] = "|";

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
}

/*
  displayboardw() will print the board in the center of the window
*/
void displayboardw(WINDOW *win, int row, int col) {
  int boardheight = (int)(sizeof(asciiboard) / sizeof(asciiboard[0]));
  int count = (int) - ((sizeof(asciiboard) / sizeof(asciiboard[0])) / 2);
  int currow; // current printing row

  // current printing column
  int currcol = (col - 30)/2; 

  // Printing the board
  for(int i = 0; i < boardheight; i++) {
    currow = (row / 2) + count;
    mvwprintw(win, currow, currcol, "%s", asciiboard[i]);
    count++;
  }

  // spacing between the elements
  int spaces = 2;

  // selected row
  int selected = 0;

  // user choice
  int key;

  // enable arrow keys usage
  keypad(win, true);

  // Print the switchable column indexes
  while(1) {
    for(int i = 0; i < WIDTH; i++) {
      char sindex[3];
      sprintf(sindex, "%d", (i + 1));
      if (i == selected)
        wattron(win, A_REVERSE);
      mvwprintw(win, currow, (currcol + spaces), sindex);
      wattroff(win, A_REVERSE);
      
      // Set spaces to 4 after printing 1
      spaces += 4;
    }

    // Reset spaces
    spaces = 2;

    // Reset cursor position
    wmove(win, currow, (currcol + spaces));
    
    // Invisible cursor
    curs_set(0);

    // Getting the keys inputs 
    key = wgetch(win);

    switch(key) {
      case KEY_RIGHT:
        if(selected == (WIDTH - 1))
          break;
        selected++;
        break;
      case KEY_LEFT:
        if (selected == 0)
          break;
        selected--;
        break;
      default:
        break;
    }

    // On ENTER 
    if (key == 10) {
      curs_set(2);
      endwin();
    }
  }


}

int main() {

  // The number of rows and columns on the screen
  int row, col, wrow, wcol;

  initscr();
  refresh();

  getmaxyx(stdscr, row, col);

  wrow = round(row * 0.8); // 80% the height
  wcol = round(col * 0.8); // 80% the width

  WINDOW *win = newwin(wrow, wcol, round((row * 0.11)), round(row * 0.33));

  int gametype = mainmenuview(win, wrow, wcol);

  int game_ongoing = true;

  clear();
  refresh();

  drawboard(); // Create the ascii version
  displayboardw(win, wrow, wcol);

  wgetch(win);
  endwin();

  return 0;
}
