#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gameplay.h"
#include "views.h"
#include "window_co.h"
#include "mcts.h"

WINDOW *game_window = NULL;

// Game window coordinates
Window_Co gwc;

int print_row;

// Beginning will switch to 1
int current_player = 2;

// Initializing the board
int board[HEIGHT][WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}};

char ascii_board[ASCII_HEIGHT][ASCII_WIDTH];

/**
 * draw the ascii version of board
 * in asciiboard
 */
void draw_board()
{
  char emptytemp[] = "| _ ";
  char xtemp[] = "| X ";
  char otemp[] = "| O ";
  char rowend[] = "|";

  char asciirow[ASCII_WIDTH];

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      if (board[i][j] == 1)
      {
        strcat(asciirow, xtemp);
      }
      else if (board[i][j] == 2)
      {
        strcat(asciirow, otemp);
      }
      else
      {
        strcat(asciirow, emptytemp);
      }
    }
    strcat(asciirow, rowend);
    strcpy(ascii_board[i], asciirow);
    strcpy(asciirow, ""); // Reinitialize the row
  }
}

int get_user_choice()
{
  int spaces = SMALL_SPACE_R;
  int selected = 0;

  // enable arrow keys
  keypad(game_window, true);

  while (1)
  {
    for (int i = 0; i < WIDTH; i++)
    {
      char sindex[3];
      sprintf(sindex, "%d", (i + 1));
      if (i == selected)
        wattron(game_window, A_REVERSE);
      mvwprintw(game_window, print_row, (gwc.c_col + spaces), sindex);
      wattroff(game_window, A_REVERSE);

      spaces += BIG_SPACE_R;
    }

    // reset spaces
    spaces = SMALL_SPACE_R;

    // make the cursor invisible
    curs_set(0);

    // getting the keys inputs
    int key = wgetch(game_window);

    switch (key)
    {
    case KEY_RIGHT:
      if (selected == (WIDTH - 1))
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
    if (key == 10)
    {
      // return player choice
      return selected;
    }
  }
}

int is_tie()
{
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      if (!board[i][j])
        return 0;
    }
  }
  return 1;
}

/**
 * prints board and display in the middle of the screen
 */
void print_ascii_board()
{
  int count = (int)-((sizeof(ascii_board) / sizeof(ascii_board[0])) / 2);

  int temp_row;
  for (int i = 0; i < ASCII_HEIGHT; i++)
  {
    temp_row = gwc.c_row + count;
    mvwprintw(game_window, temp_row, gwc.c_col, "%s", ascii_board[i]);
    count++;
  }

  print_row = temp_row;
}

void print_logs(char message[])
{
  mvwprintw(game_window, (print_row + BIG_SPACE_R), (gwc.c_col + BIG_SPACE_C), message);
}

void log_message(int winner)
{
  char message[50];

  sprintf(message, "Player %d's turn", current_player);
  print_logs(message);

  if (winner)
  {
    sprintf(message, "Player %d WON!", winner);
    print_logs(message);
  }
}

int game_view()
{
  box(game_window, 0, 0);

  draw_board();
  print_ascii_board();
  log_message(0);

  move(gwc.max_rows, gwc.max_cols);
  refresh();

  return get_user_choice();
}

void add_placement(int placement)
{
  for (int i = HEIGHT - 1; i > -1; i--)
  {
    if (board[i][placement] == 0)
    {
      board[i][placement] = current_player;
      return;
    }
  }

  return add_placement(game_view());
}

void game_over(int winner)
{
  char message[50];

  if (!winner)
  {
    strcpy(message, "Game Tied !");
  }
  else
  {
    sprintf(message, "Player %d Won !", winner);
  }

  clear();
  refresh();
  draw_board();
  print_ascii_board();
  print_logs(message);

  wgetch(game_window);
  endwin();
}

int check_winner(int b[][WIDTH])
{

  // vertical check
  for (int i = 0; i < HEIGHT - 3; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      if (b[i][j] && b[i][j] == b[i + 1][j] && b[i + 1][j] == b[i + 2][j] && b[i + 2][j] == b[i + 3][j])
        return b[i][j];
    }
  }

  // horizontal check
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH - 3; j++)
    {
      if (b[i][j] && b[i][j] == b[i][j + 1] && b[i][j + 1] == b[i][j + 2] && b[i][j + 2] == b[i][j + 3])
        return b[i][j];
    }
  }
  // ascending diagonal check
  for (int i = 3; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH - 3; j++)
    {
      if (b[i][j] && b[i][j] == b[i - 1][j + 1] && b[i - 1][j + 1] == b[i - 2][j + 2] && b[i - 2][j + 2] == b[i - 3][j + 3])
        return b[i][j];
    }
  }
  // descending diagonal check
  for (int i = 3; i < HEIGHT; i++)
  {
    for (int j = 3; j < WIDTH; j++)
    {
      if (b[i][j] && b[i][j] == b[i - 1][j - 1] && b[i - 1][j - 1] == b[i - 2][j - 2] && b[i - 2][j - 2] == b[i - 3][j - 3])
        return b[i][j];
    }
  }

  return 0;
}

void switch_players()
{
  current_player = current_player == 1 ? 2 : 1;
}

void init_game_window()
{

  clear();
  refresh();

  getmaxyx(stdscr, gwc.max_rows, gwc.max_cols);

  // Number of rows and columns
  gwc.n_rows = round(gwc.max_rows * 0.7);
  gwc.n_cols = round(gwc.max_cols * 0.5);

  // Starting coordinates
  gwc.begin_y = round(gwc.max_rows * 0.1);
  gwc.begin_x = round(gwc.max_cols * 0.25);

  // center row and col
  gwc.c_row = round((gwc.max_rows / 2) * 0.7);
  gwc.c_col = round((gwc.max_cols / 2) * 0.33);

  game_window = newwin(gwc.n_rows, gwc.n_cols, gwc.begin_y, gwc.begin_x);
}

void gameplay(int withAi)
{
  init_game_window();

  int game_ongoing = 1;

  while (game_ongoing)
  {
    switch_players();
    int player_choice = withAi && current_player == 2 ? ai_choice() : game_view();
    add_placement(player_choice);
    int winner = check_winner(board);
    if (winner)
      return game_over(winner);
    if (is_tie())
      return game_over(0);
  }
}

void ai_gameplay()
{
  init_game_window();

  int game_ongoing = 1;

  while (game_ongoing)
  {
    int player_choice = current_player == 1 ? game_view() : ai_choice();
    add_placement(player_choice);
    int winner = check_winner(board);
    if (winner)
      return game_over(winner);
    if (is_tie())
      return game_over(0);
  }
}
