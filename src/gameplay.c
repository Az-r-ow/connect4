#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gameplay.h"

WINDOW *gamewin = NULL;

int row, col, c_row, c_col;

int current_player = 1;

// Initializing the board
int board[HEIGHT][WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}};

char ascii_board[ASCII_HEIGHT][ASCII_WIDTH];

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
}

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
  keypad(gamewin, true);

  while (1)
  {
    for (int i = 0; i < WIDTH; i++)
    {
      char sindex[3];
      sprintf(sindex, "%d", (i + 1));
      if (i == selected)
        wattron(gamewin, A_REVERSE);
      mvwprintw(gamewin, c_row, (c_col + spaces), sindex);
      wattroff(gamewin, A_REVERSE);

      spaces += BIG_SPACE_R;
    }

    // reset spaces
    spaces = SMALL_SPACE_R;

    // make the cursor invisible
    curs_set(0);

    // getting the keys inputs
    int key = wgetch(gamewin);

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

  for (int i = 0; i < ASCII_HEIGHT; i++)
  {
    c_row = (row / 2) + count;
    mvwprintw(gamewin, c_row, c_col, "%s", ascii_board[i]);
    count++;
  }
}

void print_logs()
{
  mvwprintw(gamewin, (c_row + BIG_SPACE_R), (c_col + BIG_SPACE_C), "Player %d's turn !", current_player);
}

int game_view()
{
  box(gamewin, 0, 0);
  int boardheight = (int)(sizeof(ascii_board) / sizeof(ascii_board[0]));

  print_ascii_board();

  c_row = (row / 2) + (boardheight / 2);
  print_logs();
  return get_user_choice();
}

int check_gameover()
{

  // horizontalCheck
  for (int j = 0; j < HEIGHT - 3; j++)
  {
    for (int i = 0; i < WIDTH; i++)
    {
      if (board[i][j] == current_player && board[i][j + 1] == current_player && board[i][j + 2] == current_player && board[i][j + 3] == current_player)
      {
        return 1;
      }
    }
  }
  // verticalCheck
  for (int i = 0; i < WIDTH - 3; i++)
  {
    for (int j = 0; j < HEIGHT; j++)
    {
      if (board[i][j] == current_player && board[i + 1][j] == current_player && board[i + 2][j] == current_player && board[i + 3][j] == current_player)
      {
        return 1;
      }
    }
  }
  // ascendingDiagonalCheck
  for (int i = 3; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH - 3; j++)
    {
      if (board[i][j] == current_player && board[i - 1][j + 1] == current_player && board[i - 2][j + 2] == current_player && board[i - 3][j + 3] == current_player)
        return 1;
    }
  }
  // descendingDiagonalCheck
  for (int i = 3; i < HEIGHT; i++)
  {
    for (int j = 3; j < WIDTH; j++)
    {
      if (board[i][j] == current_player && board[i - 1][j - 1] == current_player && board[i - 2][j - 2] == current_player && board[i - 3][j - 3] == current_player)
        return 1;
    }
  }
  return 0;
}

void switch_players()
{
  current_player = current_player == 1 ? 2 : 1;
}

int gameplay()
{
  clear();
  refresh();

  getmaxyx(stdscr, row, col);

  int new_row = round(row * 0.7);
  int new_col = round(col * 0.5);

  gamewin = newwin(new_row, new_col, round((new_row * 0.11)), round((new_col * 0.5)));

  // Center points
  c_col = (col - 30) / 2;

  int game_ongoing = 1;

  while (game_ongoing)
  {
    switch_players();
    int player_choice = game_view();
    add_placement(player_choice);
    game_ongoing = check_gameover() ? 0 : 1;
    if (is_tie())
      return 0;
  }

  return current_player;
}
