#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "views.h"
#include "gameplay.h"
#include "mcts.h"

int main()
{
  initscr();
  refresh();
  main_menu_view();

  // ai_test();
  return 0;
}
