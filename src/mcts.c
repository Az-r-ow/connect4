#include <stdlib.h>
#include "gameplay.h"
#include "mcts.h"

int initial_state[HEIGHT][WIDTH];

int place_action(Node node)
{
  for (int i = HEIGHT - 1; i > -1; i--)
  {
    if (initial_state[i][node.action] == 0)
    {
      initial_state[i][node.action] = node.player;
      return 1;
    }
  }

  return 0;
}

void backpropagation()
{
}

void simulation()
{
}

void expansion(Node leaf)
{
  // Check if node ends game
  place_action(leaf);
  if (check_winner(initial_state))
  {
    // todo: do something
  }
}

Node selection(Node *root)
{
  if (!root.childNodes[0])
    return root;

  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    selection(&(*root.childNodes[i]));
  }
}

void initialize_state()
{
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      initial_state[i][j] = board[i][j]
    }
  }
}

Node initialize_root_node()
{
  Node root;
  root.parentNode = NULL;

  // Initializing and pointing to the children
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    Node childNode;
    childNode.parentNode = &root;
    childNode.games_played = 0;
    childNode.num_wins = 0;
    childNode.ucb1 = 0;
    childNode.action = i + 1;
    childNode.player = 2;
    root.childNodes[i] = &childNode;
  }

  root.ucb1 = 0;
  root.games_played = 0;
  root.num_wins = 0;

  return root;
}

void mcts()
{
  Node root = initialize_root_node();
  for (int i = 0; i < ITERATIONS; i++)
  {
    Node leaf = selection(&root);
    expansion(leaf);
    simulation();
    backpropagation();
  }
}

int ai_choice()
{
  return rand() % WIDTH + 1;
}