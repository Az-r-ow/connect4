#ifndef _MCTS_H_
#define _MCTS_H_

#define MAX_CHILD_NODES_NUM 7
#define ITERATIONS 1000
#define C 1.4

typedef struct Node Node;

typedef struct Node
{
  Node *parentNode;
  Node *childNodes;
  double uct;
  int player;
  int action;
  int num_wins;
  int num_visits;
} Node;

// prototype
int ai_choice();

#endif