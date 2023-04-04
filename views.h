
void comingsoonview(WINDOW *win, int row, int col)
{
  char comingsoontext[] = "Coming Soon...";

  wclear(win);
  wrefresh(win);
  mvwprintw(win, (row / 2), (col / 2) - (strlen(comingsoontext) / 2), "%s", comingsoontext);
}

void help_view(WINDOW *win, int row, int col)
{
  wclear(win);
  wrefresh(win);
  char help_title[] = "Welcome to Terminal Connect 4";

  char help_body[20][500] = {
      "This is a two player game and hopefully one day you will be able to play against an AI.",
      "But for now, this is how you play :"};

  mvwprintw(win, round(row * 0.11) + 1, (col / 2) - (strlen(help_title) / 2), "%s", help_title);
  for (int i = 0; i < 2; i++)
  {
    mvwprintw(win, round(row * 0.11) + 3 + i, round(col * 0.09), "%s", help_body[i]);
  }
}

// Create the main menu and return user game type
int mainmenuview(WINDOW *win, int row, int col)
{

  // First check if the window size
  if (row <= 25 || col <= 50)
  {
    printw("window too  small");
    getch();
    endwin();
    return 0;
  }

  box(win, 0, 0);

  char options[5][50] = {
      "2 Players",
      "Coming Soon...",
      "Help",
      "Exit"};

  // Number of options
  int optionslen = (int)(sizeof(options) / sizeof(options[0]));

  // The separation between each item
  int spacing = 2;

  // The selected element
  int selected = 0; // The first option on the list

  // The user's choice
  int choice;

  // Makes it so we can use arrow keys
  keypad(win, true);

  while (1)
  {
    for (int i = 0; i < optionslen; i++)
    {
      if (i == selected)
        wattron(win, A_REVERSE);
      mvwprintw(win, 0, spacing, options[i]);
      wattroff(win, A_REVERSE);
      spacing += strlen(options[i]) + 2;
    }
    // Reset the spacing
    spacing = 2;

    choice = wgetch(win);

    switch (choice)
    {
    case KEY_RIGHT:
      if (selected == (optionslen - 2))
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
    if (choice == 10)
    {
      switch (selected)
      {
      case 0:
        // Start the game
        return 1;
      case 1:
        // Show the coming soon view
        comingsoonview(win, row, col);
        mainmenuview(win, row, col);
        return 0;
      case 2:
        // show the help view
        help_view(win, row, col);
        mainmenuview(win, row, col);
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
