#include <stdlib.h>
#include "gameplay.h"

int ai_choice()
{
  return rand() % WIDTH + 1;
}