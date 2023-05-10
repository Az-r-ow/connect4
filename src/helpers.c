#include <curses.h>
#include <stdlib.h>
#include "helpers.h"

/**
 * Exit the curses program
 */
void exit_curses(int code)
{
  endwin();
  exit(code);
}

/**
 * Press any key to exit the program
 */
void getch_exit_curses(int code)
{
  getch();
  endwin();
  exit(code);
}

/**
 * Window Reset to clear the content of a window
 */
void wreset(WINDOW *win)
{
  wclear(win);
  wrefresh(win);
}