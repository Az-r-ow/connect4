#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "views.h"
#include "gameplay.h"

int main()
{

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

  gameplay(win, wrow, wcol);

  wgetch(win);
  endwin();

  return 0;
}
