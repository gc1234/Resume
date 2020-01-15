//This code based from:
//https://github.com/skorks/c-linked-list

#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER

typedef struct Node
{
    int *state; 
    int gn; //goal
    int hn; // heuristic
    int fn; // goal + heuristic
    struct Node *parent;
    struct Node * next;
} Node;

typedef struct List {
  Node * head;
  int size;
} List;

Node * create_node(int *state);
List * makelist();
//void display(Node *node, int *size, List * list);
void print_solution_path(Node *node, int *size);
void destroy(List * list);
bool compare_states(Node *state1, Node *state2, int *size);
void print_node_state(Node *node, int *size);
Node* remove_first(List *list);
void add_node(Node *node, int *size, List *list);
void print_path_to_goal(Node* node, int *size);
bool node_exists(Node *node, List *list, int *size);

#endif