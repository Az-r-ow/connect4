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

/**
 * changes the board based on the player placement
*/
void addplacement(int player, int placement) {
  for (int i = HEIGHT - 1; i > - 1; i--) {
    if(board[i][placement] == 0) {
      board[i][placement] = player;
      return;
    }
  }
}

/**
 * draw the ascii version of board 
 * in asciiboard
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

int getuserchoice(WINDOW* win, int currow, int curcol) {
  int spaces = 2;
  int selected = 0;

  // enable arrow keys
  keypad(win, true);

  while(1) {
    for(int i = 0; i < WIDTH; i++) {
      char sindex[3];
      sprintf(sindex, "%d", (i + 1));
      if (i == selected)
        wattron(win, A_REVERSE);
      mvwprintw(win, currow, (curcol + spaces), sindex);
      wattroff(win, A_REVERSE);

      spaces += 4;
    }

    // reset spaces
    spaces = 2;

    // make the cursor invisible
    curs_set(0);

    // getting the keys inputs
   int key = wgetch(win);

    switch(key) {
      case KEY_RIGHT:
        if(selected == (WIDTH - 1))
          break;
        selected++;
        break;
      case KEY_LEFT:
        if(selected == 0)
          break;
        selected--;
        break;
      default:
        break;
    }

    // On ENTER
    if (key == 10) {
      // return player choice
      return selected;
    }
  }
}

/**
 * prints board and display in the middle of the screen
*/
void printasciiboard(WINDOW* win, int row, int* currow, int curcol) {
  int count = (int) - ((sizeof(asciiboard) / sizeof(asciiboard[0])) / 2);

  for(int i = 0; i < ASCII_HEIGHT; i++) {
    (*currow) = (row / 2) + count;
    mvwprintw(win, (*currow), curcol, "%s", asciiboard[i]); 
    count++;
  }
}

void gameplay(WINDOW* win, int row, int col) {
  int player = 1;
  int boardheight = (int)(sizeof(asciiboard) / sizeof(asciiboard[0]));
  int currow; // current printing row

  // current printing column
  int curcol = (col - 30) / 2;

  int playerchoice = 0;

  do
  {
    player = player == 1 ? 2 : 1;
    drawboard();
    printasciiboard(win, row, &currow, curcol);
    currow = (row / 2) + (boardheight / 2);
    playerchoice = getuserchoice(win, currow, curcol);
    addplacement(player, playerchoice);
  } while (1);
}

