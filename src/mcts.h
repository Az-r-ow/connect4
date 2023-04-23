#ifndef _MCTS_H_
#define _MCTS_H_

#define MAX_CHILD_NODES_NUM 7
#define ITERATIONS 1000

typedef struct Node Node;

typedef struct Node
{
  Node *parentNode;
  Node *childNodes;
  int games_played;
  int num_wins;
} Node;

// prototype
int ai_choice();

#endif