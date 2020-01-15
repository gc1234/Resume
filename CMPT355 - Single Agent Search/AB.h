#ifndef AB_H
#define AB_H

void calc_hn(Node *node, int *len, int *solution);
void calc_fn(Node *node);
void calc_gn(Node *prev_node, Node *new_node);
int populate_disks(int *disks, char *input, int size);
int handle_input(int *smallDisks, int *largeDisks, int size);
int* clone_smallDisks(int *smallDisks, int *size);
void generate_new_states(int *largeDisks, Node *smallDisks, int *size, List *frontier, Dictionary explored_and_frontier, int *solution);
int compareState(const void *a, const void *b);
int* solution_state(int *state, int *size);

#endif