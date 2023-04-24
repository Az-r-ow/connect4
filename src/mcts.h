#ifndef _MCTS_H_
#define _MCTS_H_

#define MAX_CHILD_NODES_NUM 7
#define ITERATIONS 1000
#define C 2

typedef struct Node Node;

typedef struct Node
{
  Node *parentNode;
  Node *childNodes[MAX_CHILD_NODES_NUM];
  double ucb1;
  int player;
  int action;
  int games_played;
  int num_wins;
  int num_visits;
} Node;

// prototype
int ai_choice();
void ai_test();

#endif