#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gameplay.h"
#include "mcts.h"

int initial_state[HEIGHT][WIDTH];

Node choose_random_node(Node parent)
{
  int rand_index = rand() % MAX_CHILD_NODES_NUM + 1;
  return parent.childNodes[rand_index];
}

double calculate_ucb1(Node node)
{
  int N = (node.parentNode)->num_visits;
  printf("N -> %d\n", N);
  if (!N)
    return 0;
  return node.ucb1 + (C * (sqrt(log(N) / node.num_visits)));
}

void free_tree(Node *root)
{
  if (root->childNodes == NULL)
    return;

  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    free_tree(&root->childNodes[i]);
  }

  free(root->childNodes);
  return;
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

void init_child_node(Node *parentNode, int action)
{
  printf("A node is being created !\n");

  Node childNode;

  childNode.ucb1 = 0;
  childNode.player = parentNode->player == 1 ? 2 : 1;
  childNode.action = action;
  childNode.games_played = 0;
  childNode.num_wins = 0;
  childNode.num_visits = 0;

  childNode.childNodes = NULL;
  childNode.parentNode = &(*parentNode);
  parentNode->childNodes[action] = childNode;

  return;
}

Node initialize_root_node()
{
  Node root;
  root.parentNode = NULL;

  root.childNodes = malloc(MAX_CHILD_NODES_NUM * sizeof(Node));
  root.action = 0;
  root.uct = 0;
  root.games_played = 0;
  root.num_wins = 0;
  root.num_visits = 0;
  root.player = 1;

  // Initializing and pointing to the children
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    init_child_node(&root, i);
  }

  return root;
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
    init_child_node(&(*leaf), i);
  }

  // Choose a random child node to simulate
  Node c = choose_random_node(*leaf);
  simulation(&c);
}

Node selection(Node *node)
{
  // Check node is leaf
  if (!node->childNodes)
    return (*node);

  // Calculate ucb for child nodes
  Node *highestUcbNode = NULL;
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    (node->childNodes[i]).ucb1 = calculate_ucb1(node->childNodes[i]);

    if (!highestUcbNode)
    {
      highestUcbNode = &node->childNodes[i];
      printf("HighestUcbNode -> %f\n", highestUcbNode->ucb1);
      continue;
    }

    // Find node with highest ucb1
    if ((node->childNodes[i]).ucb1 > highestUcbNode->ucb1)
    {
      printf("Found a higher ucb node : \n");
      highestUcbNode = &node->childNodes[i];
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

void mcts()
{
  Node root = initialize_root_node();
  for (int i = 0; i < ITERATIONS; i++)
  {
    Node leaf = selection(&root);
    expansion(&leaf);
    return;
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
  for (int i = 0; i < ITERATIONS; i++)
  {
    Node leaf = selection(&root);
    printf("leaf action => %d\n", leaf.action);
    return;
    // expansion(&leaf);
    // simulation();
    // backpropagation();
  }
}
