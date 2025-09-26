#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <curses.h>

// Prototypes
void exit_curses(int code);
void getch_exit_curses(int code);
void wreset(WINDOW *win);

#endif
