#ifndef _MCTS_H_
#define _MCTS_H_

#define MAX_CHILD_NODES_NUM 7
#define ITERATIONS 4000 /* Higher will lead to better AI moves */
#define C 1.4           /* Almost root(2) */

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