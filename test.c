#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
  char finalrow[] = "  1   2   3   4   5   6   7  ";
  strcpy(asciiboard[ASCII_HEIGHT - 1], finalrow);
}

/*
  printboardw() will print the board in the center of the window
*/
void printboardw(WINDOW *win, int row, int col) {
  int boardheight = (int)(sizeof(asciiboard) / sizeof(asciiboard[0]));
  int count = (int) - ((sizeof(asciiboard) / sizeof(asciiboard[0])) / 2);
  for(int i = 0; i < boardheight; i++) {
    mvwprintw(win, (row/ 2) + count, (col - 30)/2, "%s", asciiboard[i]);
    count++;
  }
}

void printinterface(WINDOW *win, int wrow, int wcol) {
    box(win, 0, 0);
    printboardw(win, wrow, wcol);
}

void handlemainmenu(WINDOW *win) {

    char options[5][50] = {
        "2 Players",
        "Coming Soon...",
        "Help"
    };

    // Number of options
    int optionslen = (int) (sizeof(options) / sizeof(options[0]));

    // The separation between each item
    int spacing = 2;

    // The selected element
    int selected = 0; // The first option on the list

    // The user's choice
    int choice;

    // Makes it so we can use arrow keys
    keypad(win, true);

    while(1) {
      for (int i = 0; i < optionslen; i++) {
          if (i == selected)
            wattron(win, A_REVERSE);
          mvwprintw(win, 0, spacing, options[i]);
          wattroff(win, A_REVERSE);
          spacing += strlen(options[i]) + 2;
      }
      // Reset the spacing
      spacing = 2;

      choice = wgetch(win);

      switch (choice) {
        case KEY_RIGHT:
          if(selected == (optionslen - 3))
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

      if (choice == 10)
        break;
    }

}

int main() {

  // The number of rows and columns on the screen
  int row, col, wrow, wcol;

  drawboard();

  initscr();
  getmaxyx(stdscr, row, col);

  wrow = round(row * 0.8); // 80% the height
  wcol = round(col * 0.8); // 80% the width

  WINDOW *win = newwin(wrow, wcol, round((row * 0.11))  , round(row * 0.33));
  printinterface(win, wrow, wcol);
  handlemainmenu(win);

  curs_set(0);
  wgetch(win);
  endwin();

  return 0;
}
