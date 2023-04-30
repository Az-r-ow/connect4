#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gameplay.h"
#include "mcts.h"

int current_state[HEIGHT][WIDTH];
int ai_player;

Node *get_random_child(Node *parent)
{
  int rand_index = rand() % MAX_CHILD_NODES_NUM;
  return &(parent->childNodes[rand_index]);
}

int get_random_available_action(int b[][WIDTH])
{
  int *available_actions;
  int available_actions_count = 0;

  for (int i = 0; i < WIDTH; i++)
  {
    if (!b[0][i])
    {
      available_actions_count++;
    }
  }

  if (available_actions_count == WIDTH)
    return rand() % MAX_CHILD_NODES_NUM + 1;

  available_actions = malloc(sizeof(int) * available_actions_count);

  int index = 0;

  for (int i = 0; i < WIDTH; i++)
  {
    if (!b[0][i])
    {
      available_actions[index] = i;
      index++;
    }
  }

  int rand_index = rand() % available_actions_count;
  return available_actions[rand_index];
}

double calculate_uct(Node node)
{
  int N = (node.parentNode)->num_visits;

  if (!N || !node.num_wins)
    return INFINITY;

  double V = node.player == ai_player ? (node.num_wins / node.num_visits) : -(node.num_wins / node.num_visits);
  return V + (C * (sqrt(log(N) / node.num_visits)));
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

int place_action_from_node(Node *node)
{
  for (int i = HEIGHT - 1; i > -1; i--)
  {
    if (current_state[i][node->action] == 0)
    {
      current_state[i][node->action] = node->player;
      return 1;
    }
  }

  return 0;
}

int get_optimal_move(Node *node)
{
  Node *highestUctNode = NULL;

  for (int i = 0; i < WIDTH; i++)
  {
    if (highestUctNode == NULL)
    {
      highestUctNode = &(node->childNodes[i]);
      continue;
    }

    if (highestUctNode->uct <= (node->childNodes[i]).uct)
    {
      highestUctNode = &(node->childNodes[i]);
    }
  }

  return highestUctNode->action;
}

void init_child_node(Node *parentNode, int action)
{
  Node childNode;

  // printf("parent node => %d\n", parentNode->player);
  childNode.uct = 0;
  childNode.player = parentNode->player == 1 ? 2 : 1;
  childNode.action = action;
  childNode.num_wins = 0;
  childNode.num_visits = 0;

  childNode.childNodes = NULL;
  childNode.parentNode = &(*parentNode);
  parentNode->childNodes[action] = childNode;

  // printf("initializing child node for parent node => %d to child node => %d \n", parentNode->player, childNode.player);
  // printf("child node => %d\n", childNode.player);

  return;
}

Node initialize_root_node()
{
  Node root;
  root.parentNode = NULL;

  root.childNodes = malloc(MAX_CHILD_NODES_NUM * sizeof(Node));

  root.action = 0;
  root.uct = 1;
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

void backpropagation(Node *node, int result)
{
  node->num_visits = node->num_visits + 1;
  node->num_wins = result == node->player ? node->num_wins + 1 : node->num_wins;

  // Root node
  if (node->parentNode == NULL)
  {
    printf("End of backpropagation\n");
    for (int i = 0; i < HEIGHT; i++)
    {
      for (int j = 0; j < WIDTH; j++)
      {
        printf("%d ", current_state[i][j]);
      }
      printf("\n");
    }
    return;
  }

  backpropagation(node->parentNode, result);
  return;
}

void simulation(Node *c)
{
  place_action_from_node(c);
  int winner = 0;
  int tied_game = 0;

  Node random_action_node = {
      .player = c->player};

  // ROLLOUT
  while (!winner && !tied_game)
  {
    // Generate random action
    random_action_node.action = get_random_available_action(current_state);
    random_action_node.player = random_action_node.player == 1 ? 2 : 1;

    place_action_from_node(&random_action_node);

    // Check game ended
    winner = check_winner(current_state);
    tied_game = is_tie(current_state);
  }

  int result = winner ? winner : 0;
  backpropagation(c, result);
  return;
}

void expansion(Node *leaf)
{

  // Check if node ends game
  int winner = check_winner(current_state);
  int tied_game = is_tie(current_state);

  if (winner || tied_game)
  {
    int result = winner ? winner : 0;
    backpropagation(leaf, result);
    return;
  }

  // Allocate memory for child nodes
  leaf->childNodes = malloc(MAX_CHILD_NODES_NUM * sizeof(Node));

  // Create child nodes
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    init_child_node(leaf, i);
  }

  // Choose a random child node to simulate
  Node *c = get_random_child(leaf);
  simulation(c);
}

Node *selection(Node *node)
{
  // Check node is leaf
  if (!node->childNodes)
    return node;

  // Calculate ucb for child nodes
  Node *highestUcbNode = NULL;
  int equal_node_count = 0;

  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    (node->childNodes[i]).uct = calculate_uct(node->childNodes[i]);
    printf("Action %d ucb => %f\n", i, (node->childNodes[i]).uct);
    printf("Action %d num visits => %d\n", i, (node->childNodes[i]).num_visits);
    printf("Action %d num wins => %d\n", i, (node->childNodes[i]).num_wins);

    if (!highestUcbNode)
    {
      equal_node_count++;
      highestUcbNode = &node->childNodes[i];
      // printf("HighestUcbNode -> %f\n", highestUcbNode->uct);
      continue;
    }

    if ((node->childNodes[i]).uct >= highestUcbNode->uct)
    {
      equal_node_count++;
      // printf("Found a higher ucb node : \n");
      highestUcbNode = &node->childNodes[i];
    }
  }

  if (equal_node_count == MAX_CHILD_NODES_NUM)
  {
    Node *random_child_node = get_random_child(node);
    place_action_from_node(random_child_node);
    return selection(random_child_node);
  }

  printf("Highest node action %d for player %d taken => %f\n", highestUcbNode->action, node->player, highestUcbNode->uct);

  // Update the board as we go deeper in the tree
  place_action_from_node(highestUcbNode);

  // select node with highest ucb
  return selection(highestUcbNode);
}

void initialize_state()
{
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      current_state[i][j] = board[i][j];
    }
  }
}

void traverse_tree(Node *node)
{
  printf("node => %p\n", node->childNodes);
  if (!node->childNodes)
    return;

  printf("Node player => %d\n", node->player);
  for (int i = 0; i < MAX_CHILD_NODES_NUM; i++)
  {
    printf("ChildNode player => %d\n", (node->childNodes[i]).player);
    printf("ChildNode visits => %d\n", (node->childNodes[i]).num_visits);
    printf("ChildNode wins => %d\n", (node->childNodes[i]).num_wins);
    traverse_tree(&(node->childNodes[i]));
  }
  return;
}

// Returns a monte carlo tree
Node mcts()
{
  Node root = initialize_root_node();
  ai_player = root.player == 1 ? 2 : 1;
  initialize_state();

  for (int i = 0; i < ITERATIONS; i++)
  {
    printf("Selection starting");
    Node *leaf = selection(&root);
    expansion(leaf);

    // if (i == 500)
    //   return root;

    initialize_state();
  }

  return root;
}

int ai_choice()
{
  Node mcts_tree = mcts();
  // traverse_tree(&mcts_tree);
  for (int i = 0; i < WIDTH; i++)
  {
    printf("ChildNode %d player %d\n", i, (mcts_tree.childNodes[i]).player);
    printf("Num visits => %d Num wins => %d UCT => %f\n", (mcts_tree.childNodes[i]).num_visits, (mcts_tree.childNodes[i]).num_wins, (mcts_tree.childNodes[i]).uct);
  }
  printf("root node num visits => %d\n", mcts_tree.num_visits);
  printf("root node num wins => %d\n", mcts_tree.num_wins);
  printf("root node num wins => %f\n", mcts_tree.uct);

  int optimal_move = get_optimal_move(&mcts_tree);
  free_tree(&mcts_tree);
  return optimal_move;
}

void ai_test()
{
  int ai = ai_choice();
  printf("Ai choice => %d \n", ai);
}
