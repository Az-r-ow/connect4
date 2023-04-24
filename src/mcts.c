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
  printf("Node's parent action -> %d\n", (node.parentNode)->action);
  printf("Node's action -> %d \n", node.action);
  int N = (node.parentNode)->num_visits;
  printf("N -> %d\n", N);
  if (!N)
    return 0;
  return node.ucb1 + (C * (sqrt(log(N) / node.num_visits)));
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

Node init_child_node(Node *parentNode, int action)
{
  printf("A node is being created !\n");

  Node childNode;

  // setting child nodes
  set_empty_pointer_array(&childNode);
  childNode.ucb1 = 0;
  childNode.player = parentNode->player == 1 ? 2 : 1;
  childNode.action = action;
  childNode.games_played = action + 1;
  childNode.num_wins = 0;
  childNode.num_visits = 0;

  printf("That node's action -> %d\n", childNode.action);
  printf("That node's parent action -> %d\n", childNode.parentNode->action);

  // child to parent
  childNode.parentNode = &(*parentNode);

  return childNode;
}

Node initialize_root_node()
{
  Node root;
  root.parentNode = NULL;

  root.action = 0;
  root.ucb1 = 0;
  root.games_played = 0;
  root.num_wins = 0;
  root.num_visits = 0;
  root.player = 1;

  // Initializing and pointing to the children
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    Node childNode = init_child_node(&root, i);
    printf("ChildNode's action -> %d \n", childNode.action);
    root.childNodes[i] = &childNode;
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
  if (!(*node).childNodes[0])
    return (*node);

  // Calculate ucb for child nodes
  Node *highestUcbNode = NULL;
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    printf("Child Node %d ucb -> %f \n", i, (node->childNodes[i])->ucb1);
    (node->childNodes[i])->ucb1 = calculate_ucb1((*(node->childNodes[i])));

    if (highestUcbNode == NULL)
    {
      printf("I'm here ! \n");
      highestUcbNode = &(*(node->childNodes[i]));
      printf("HighestUcbNode -> %f\n", highestUcbNode->ucb1);

      continue;
    }

    printf("HighestUcbNode -> %f\n", highestUcbNode->ucb1);

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
  // Node leaf = selection(&root);

  printf("Root node action -> %d \n", root.action);
  printf("Root node player -> %d\n", root.player);

  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    printf("%d\n", (*root.childNodes[i]).action);
    printf("%d\n", (*root.childNodes[i]).games_played);

    // printf("Leaf node -> %d \n", leaf.action);
  }
}
