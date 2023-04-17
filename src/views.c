#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "views.h"
#include "window_co.h"

char menu_options[5][50] = {
    "2 Players",
    "Coming Soon...",
    "Help",
    "Exit"};

char help_title[] = "Welcome to Terminal Connect 4";

char help_body[20][500] = {
    "This is a two player game and hopefully one day you will be able to play against an AI.",
    "But for now, this is how you play :"};

WINDOW *main_window = NULL;

int user_choice = 0;

struct Window_Co view_window_co;

void coming_soon_view()
{
  char comingsoontext[] = "Coming Soon...";

  wclear(main_window);
  wrefresh(main_window);

  mvwprintw(main_window, view_window_co.c_row, view_window_co.c_col - (strlen(comingsoontext) / 2), "%s", comingsoontext);
}

void help_view()
{
  wclear(main_window);
  wrefresh(main_window);

  // center view_window_co.max_cols
  int print_col = view_window_co.c_col - (strlen(help_title) / 2);

  mvwprintw(main_window, (view_window_co.begin_y + 1), print_col, "%s", help_title);
  for (int i = 0; i < 2; i++)
  {
    mvwprintw(main_window, view_window_co.begin_y + 3 + i, round(view_window_co.max_cols * 0.09), "%s", help_body[i]);
  }
}

void window_init()
{
  getmaxyx(stdscr, view_window_co.max_rows, view_window_co.max_cols);

  // Number of rows and cols
  view_window_co.n_rows = round(view_window_co.max_rows * 0.8);
  view_window_co.n_cols = round(view_window_co.max_cols * 0.8);

  // Starting coordinates
  view_window_co.begin_y = round(view_window_co.max_rows * 0.11);
  view_window_co.begin_x = round(view_window_co.max_rows * 0.33);

  // Center view_window_co.max_rows and view_window_co.max_cols
  view_window_co.c_row = round((view_window_co.max_rows / 2) * 0.8);
  view_window_co.c_col = round((view_window_co.max_cols / 2) * 0.8);

  initscr();
  refresh();

  main_window = newwin(view_window_co.n_rows, view_window_co.n_cols, view_window_co.begin_y, view_window_co.begin_x);
}

// Create the main menu and return user game type
int main_menu_view()
{
  // Only if user no user choice
  if (!user_choice)
  {
    window_init();
  }

  // First check if the window size
  if (view_window_co.max_rows <= 25 || view_window_co.max_cols <= 50)
  {
    printw("window too  small");
    getch();
    endwin();
    return 0;
  }

  box(main_window, 0, 0);

  // Number of options
  int menu_options_len = (int)(sizeof(menu_options) / sizeof(menu_options[0]));

  // The separation between each item
  int spacing = 2;

  // The user's choice
  int choice;

  // Makes it so we can use arrow keys
  keypad(main_window, true);

  while (1)
  {
    for (int i = 0; i < menu_options_len; i++)
    {
      if (i == user_choice)
        wattron(main_window, A_REVERSE);
      mvwprintw(main_window, 0, spacing, menu_options[i]);
      wattroff(main_window, A_REVERSE);
      spacing += strlen(menu_options[i]) + 2;
    }
    // Reset the spacing
    spacing = 2;

    choice = wgetch(main_window);

    switch (choice)
    {
    case KEY_RIGHT:
      if (user_choice == (menu_options_len - 2))
        break;
      user_choice++;
      break;
    case KEY_LEFT:
      if (user_choice == 0)
        break;
      user_choice--;
      break;
    default:
      break;
    }

    // On ENTER
    if (choice == 10)
    {
      switch (user_choice)
      {
      case 0:
        // Start the game
        return 1;
      case 1:
        // Show the coming soon view
        coming_soon_view();
        main_menu_view();
        return 0;
      case 2:
        // show the help view
        help_view();
        main_menu_view();
        return 0;
      case 3:
        // Leave the game
        refresh();
        exit(0);
        return 0;
      }
    }
  }
}
