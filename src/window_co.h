#ifndef _WINDOW_COORDINATES_
#define _WINDOW_COORDINATES_

// Window coordinates struct
typedef struct Window_Co
{
  // Size of terminal
  int max_rows;
  int max_cols;

  // Number in window
  int n_rows;
  int n_cols;

  // Starting coordinates
  int begin_x;
  int begin_y;

  // Center coordinates
  int c_row;
  int c_col;
} Window_Co;

#endif