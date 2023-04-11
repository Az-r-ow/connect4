// The board's boundaries
#define WIDTH 7
#define HEIGHT 6

// The ascii board boundaries
#define ASCII_HEIGHT 7
#define ASCII_WIDTH 50

#define BIG_SPACE 4
#define SMALL_SPACE 2

// Initializing the board
int board[HEIGHT][WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}};

/*
  The size of the string will be constant
  We will have 7 columns each will look like :
  | _ | = 5 chars
   7 * 5 = 35
   and since 6 of them will have a shared colum
*/
char asciiboard[ASCII_HEIGHT][ASCII_WIDTH];

/**
 * changes the board based on the player placement
 */
void addplacement(int player, int placement)
{
  for (int i = HEIGHT - 1; i > -1; i--)
  {
    if (board[i][placement] == 0)
    {
      board[i][placement] = player;
      return;
    }
  }
}

/**
 * draw the ascii version of board
 * in asciiboard
 */
void drawboard()
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
    strcpy(asciiboard[i], asciirow);
    strcpy(asciirow, ""); // Reinitialize the row
  }
}

int getuserchoice(WINDOW *win, int currow, int curcol)
{
  int spaces = SMALL_SPACE;
  int selected = 0;

  // enable arrow keys
  keypad(win, true);

  while (1)
  {
    for (int i = 0; i < WIDTH; i++)
    {
      char sindex[3];
      sprintf(sindex, "%d", (i + 1));
      if (i == selected)
        wattron(win, A_REVERSE);
      mvwprintw(win, currow, (curcol + spaces), sindex);
      wattroff(win, A_REVERSE);

      spaces += BIG_SPACE;
    }

    // reset spaces
    spaces = SMALL_SPACE;

    // make the cursor invisible
    curs_set(0);

    // getting the keys inputs
    int key = wgetch(win);

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

/**
 * prints board and display in the middle of the screen
 */
void printasciiboard(WINDOW *win, int row, int *currow, int curcol)
{
  int count = (int)-((sizeof(asciiboard) / sizeof(asciiboard[0])) / 2);

  for (int i = 0; i < ASCII_HEIGHT; i++)
  {
    (*currow) = (row / 2) + count;
    mvwprintw(win, (*currow), curcol, "%s", asciiboard[i]);
    count++;
  }
}

void printlogs(WINDOW *win, int gameongoing, int player, int currow, int curcol)
{
  mvwprintw(win, (currow + BIG_SPACE), (curcol + BIG_SPACE), "Player %d turn !", player);
}

int gameview(WINDOW *win, int gameongoing, int player, int row, int col, int currow, int curcol)
{
  box(win, 0, 0);
  int boardheight = (int)(sizeof(asciiboard) / sizeof(asciiboard[0]));

  printasciiboard(win, row, &currow, curcol);

  currow = (row / 2) + (boardheight / 2);
  printlogs(win, gameongoing, player, currow, curcol);
  return getuserchoice(win, currow, curcol);
}

int checkgameover(int player)
{

  // horizontalCheck
  for (int j = 0; j < HEIGHT - 3; j++)
  {
    for (int i = 0; i < WIDTH; i++)
    {
      if (board[i][j] == player && board[i][j + 1] == player && board[i][j + 2] == player && board[i][j + 3] == player)
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
      if (board[i][j] == player && board[i + 1][j] == player && board[i + 2][j] == player && board[i + 3][j] == player)
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
      if (board[i][j] == player && board[i - 1][j + 1] == player && board[i - 2][j + 2] == player && board[i - 3][j + 3] == player)
        return 1;
    }
  }
  // descendingDiagonalCheck
  for (int i = 3; i < HEIGHT; i++)
  {
    for (int j = 3; j < WIDTH; j++)
    {
      if (board[i][j] == player && board[i - 1][j - 1] == player && board[i - 2][j - 2] == player && board[i - 3][j - 3] == player)
        return 1;
    }
  }
  return 0;
}

int gameplay(WINDOW *win, int row, int col)
{
  int player = 1;
  int currow = row; // current printing row

  // current printing column
  int curcol = (col - 30) / 2;

  int playerchoice = 0;

  int gameongoing = 1;

  while (gameongoing)
  {
    player = player == 1 ? 2 : 1;
    drawboard();
    playerchoice = gameview(win, gameongoing, player, row, col, currow, curcol);
    addplacement(player, playerchoice);
    gameongoing = checkgameover(player) ? 0 : 1;
  }

  return player;
}
