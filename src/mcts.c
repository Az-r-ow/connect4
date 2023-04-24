#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gameplay.h"
#include "mcts.h"

int initial_state[HEIGHT][WIDTH];

Node choose_random_node(Node parent)
{
  int rand_index = rand() % MAX_CHILD_NODES_NUM + 1;
  return (*(parent.childNodes[rand_index]));
}

double calculate_ucb1(Node node)
{
  int N = (node.parentNode)->num_visits;
  if (!N)
    return 0;
  double ucb1 = node.ucb1 + (C * (sqrt(log(N) / node.num_visits)));
  // printf("N -> %f\n", log(N));
  // printf("node.num_visits -> %d\n", node.num_visits);
  // printf("node.ucb1 -> %f\n", node.ucb1);
  // printf("ucb1 -> %f\n", ucb1);
  return ucb1;
}

void set_empty_pointer_array(Node *node)
{
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    node->childNodes[i] = NULL;
  }
}

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

void simulation(Node *c)
{
}

void expansion(Node *leaf)
{
  // Check if node ends game
  place_action((*leaf));
  if (check_winner(initial_state))
  {
    // todo: do something
  }

  // Create child nodes
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    Node childNode;
    childNode.parentNode = &(*leaf);
    set_empty_pointer_array(leaf);
    childNode.ucb1 = 0;
    childNode.player = leaf->player == 1 ? 2 : 1;
    childNode.action = i;
    childNode.games_played = 0;
    childNode.num_wins = 0;
    childNode.num_visits = 0;

    leaf->childNodes[i] = &childNode;
  }

  // Choose a random child node to simulate
  Node c = choose_random_node(*leaf);
  simulation(&c);
}

Node selection(Node *node)
{
  // Check node is leaf
  if (!(*node).childNodes[0])
    return (*node);

  // Calculate ucb for child nodes
  Node *highestUcbNode = NULL;
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    (node->childNodes[i])->ucb1 = calculate_ucb1((*(node->childNodes[i])));
    if (highestUcbNode == NULL)
    {
      highestUcbNode = &(*(node->childNodes[i]));
      continue;
    }

    // Find node with highest ucb1
    if (((node->childNodes[i]))->ucb1 > highestUcbNode->ucb1)
    {
      highestUcbNode = &(*(node->childNodes[i]));
    }
  }

  // select node with highest ucb
  return selection(highestUcbNode);
}

void initialize_state()
{
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      initial_state[i][j] = board[i][j];
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
    set_empty_pointer_array(&childNode);
    childNode.games_played = 0;
    childNode.num_wins = 0;
    childNode.ucb1 = 0;
    childNode.action = i;
    childNode.player = 2;
    childNode.num_visits = 0;
    root.childNodes[i] = &childNode;
  }

  root.ucb1 = 0;
  root.games_played = 0;
  root.num_wins = 0;
  root.num_visits = 0;

  printf("Root num_wins -> %d\n", root.num_visits);

  return root;
}

void mcts()
{
  Node root = initialize_root_node();
  for (int i = 0; i < ITERATIONS; i++)
  {
    Node leaf = selection(&root);
    return;
    // expansion(&leaf);
    // simulation();
    // backpropagation();
  }
}

int ai_choice()
{
  return rand() % WIDTH + 1;
}

void ai_test()
{
  Node root = initialize_root_node();
  Node leaf = selection(&root);

  printf("Root node -> %d \n", root.action);
  printf("Leaf node -> %d \n", leaf.action);
}
