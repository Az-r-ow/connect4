#include <stdio.h>
#include <ctype.h>
#include "getstr.h"

// The board's boundaries
#define WIDTH 7
#define HEIGHT 6

// Initializing the board
int board[HEIGHT][WIDTH] = {
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0}
};


// Draw the board in the console
int draw(){
  int i = 0;

  printf("\n\n");

  for(; i < HEIGHT; i++){
    printf("|");
    for (int j = 0; j < WIDTH; j++){

      if (board[i][j] == 1){
        printf(" X ");
      }else if (board[i][j] == 2){
        printf(" O ");
      }else{
        printf(" . ");
      }
      printf("|");
    }
    printf("\n");
  }
  printf("  1   2   3   4   5   6   7 \n");
  return 0;
}

/*
  add_placement will return :
  * 0 if the user inputs a number outside the board boundaries or if the column is full
  * 1 if everything is ok
*/
int add_placement(int player, int placement){

  placement = placement - 1; //Because arrays are 0 indexed

  if (placement > WIDTH - 1 || placement < 0){
    return 0;
  }

    for (int i = HEIGHT - 1; i > - 1; i--){
      if(board[i][placement] == 0){
        board[i][placement] = player;
        return 1;
      }
    }
    return 0;
  }

int areFourConnected(int player){

    // horizontalCheck
    for (int j = 0; j<HEIGHT-3 ; j++ ){
        for (int i = 0; i<WIDTH; i++){
            if (board[i][j] == player && board[i][j+1] == player && board[i][j+2] == player && board[i][j+3] == player){
                return 1;
            }
        }
    }
    // verticalCheck
    for (int i = 0; i<WIDTH-3 ; i++ ){
        for (int j = 0; j<HEIGHT; j++){
            if (board[i][j] == player && board[i+1][j] == player && board[i+2][j] == player && board[i+3][j] == player){
                return 1;
            }
        }
    }
    // ascendingDiagonalCheck
    for (int i=3; i<HEIGHT; i++){
        for (int j=0; j<WIDTH-3; j++){
            if (board[i][j] == player && board[i-1][j+1] == player && board[i-2][j+2] == player && board[i-3][j+3] == player)
                return 1;
        }
    }
    // descendingDiagonalCheck
    for (int i=3; i<HEIGHT; i++){
        for (int j=3; j< WIDTH; j++){
            if (board[i][j] == player && board[i-1][j-1] == player && board[i-2][j-2] == player && board[i-3][j-3] == player)
                return 1;
        }
    }
    return 0;
}

int player_turn(int * player_1, int * player_2, int * last_played_by, int * current_player){
  char player1_input[2], player2_input[2];

  if (*last_played_by == 2){
    printf("Player 1 : ");

    *player_1 = getstr_to_int();

    if (!add_placement(*current_player, *player_1)){
      printf("You must input a number between 1 and 7 !\n");
      return player_turn(player_1, player_2, last_played_by, current_player);
    }

    *last_played_by = 1;
    *current_player = 2;
  }else{

    printf("Player 2 : ");

    *player_2 = getstr_to_int();

    if (!add_placement(*current_player, *player_2)){
      printf("You must input a number between 1 and 7 !\n");
      return player_turn(player_1, player_2, last_played_by, current_player);
    }

    *last_played_by = 2;
    *current_player = 1;
  }
  return 0;
}

int isTie(){
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      if(!board[i][j])
        return 0;
    }
  }
  return 1;
}

int main(){

  int game_over = 0;

  printf("\n\n"); 
  printf("On each player's turn input the number of the row where you want to drop your disk\n");
  printf("Player's input should be a number between 1 ==> 7\n");
  printf("Player 1 will be ==> X\n");
  printf("Player 2 will be ==> O\n");

  // Print the empty Board
  draw();

  // player_1 and player_2 will represent the column where the user will drop the token
  int player_1, player_2, last_played_by = 2, current_player = 1;

  while(game_over == 0){

    // Prompts user for input
    // Add the user's placement
    player_turn(&player_1, &player_2, &last_played_by, &current_player);

    // After move
    // Draw the new table
    draw();


    // Checks for Four connected
    if (last_played_by == 2 && areFourConnected(last_played_by)){
      printf("Player 2 won !\n");
      return 0;
    }

    if (last_played_by == 1 && areFourConnected(last_played_by)){
      printf("Player 1 won !\n");
      return 0;
    }

    if(isTie()){
      printf("It's a tie !\n");
      return 0;
    }

  }

  return 0;
}
