#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "hash.h"
#include "AB.h"
#define MEM_ENV_VAR "MAXMEM_MB"

/*
Sort function for qsort.
*/
int compareState(const void *a, const void *b){
    int *a1=(int *)a;
    int *b1=(int *)b;
    return *a1-*b1;
}

/*
The solution state is returned 
*/
int* solution_state(int *state, int *size){
    int *solution = malloc((*size-1)*sizeof(int));
    if (!solution) {
        fprintf(stderr, "Malloc failed\n");
        exit(EXIT_FAILURE);
    }
    int count = 0;
    for (int i=0; i<*size; i++){
        if (state[i] != 0) {
            solution[count++] = state[i];
        }
    }

    qsort(solution, *size-1, sizeof(solution[0]), compareState);
    return solution;
}

/*
absolute value difference heuristic
*/
void calc_hn(Node *node, int *size, int *solution)
{
    int temp;
    int count = 0;
    for (int i=0, k=0; i<*size; i++){
        if (node->state[i] != 0){
            temp = abs(node->state[i] - solution[k++]);
            count += temp;
        }
    }
    node->hn = count;
}

/*
* Manhatten distance heuristic
 */
/*void calc_hn(Node *node, int *size, int *solution)
{
    int count = 0;
    for (int i=0; i<*size-1; i++){
        for (int j=i; j<*size; j++){
            if (node->state[j] == 0){
                continue;
            }else if (node->state[j] == solution[i]){
                break;
            }else{
                count++;
            }
        }
    }
    //printf("count = %d\n", count);
    node->hn = count;
}*/

//calaculates the cost of the new node. New node = old node + 1
void calc_gn(Node *prev_node, Node *new_node)
{
    if (!prev_node) {
        new_node->gn++;
        return;
    }
    new_node->gn = prev_node->gn + 1;
}

//fn = goal + heuristic
void calc_fn(Node *node)
{
    node->fn = node->gn + node->hn;
}

int populate_disks(int *disks, char *input, int size)
{
    int i = 0;
    char *token;

    token = strtok(input, " ");

    while(token) {
        if (i == size) {
            fprintf(stderr, "Input given is larger than size given\n");
            return EXIT_FAILURE;
        }

        disks[i] = atoi(token);
        token = strtok(NULL, " ");
        i++;
    }

    return 0;
}

int handle_input(int *smallDisks, int *largeDisks, int size)
{
    char input[size*3];

    fgets(input, size*3, stdin);

    if ((populate_disks(largeDisks, input, size) == 1))
        return EXIT_FAILURE;

    fgets(input, size*3, stdin);

    if ((populate_disks(smallDisks, input, size) == 1))
        return EXIT_FAILURE;

    return 0;
}

/*
Iterates through all the values in the largeDisk. At each 
each iteration all the possible swaps are created from 
the smallDisks.
*/
void generate_new_states(int *largeDisks, Node *smallDisks, int *size, List *frontier, Dictionary explored_and_frontier, int *solution){
    int possible_moves[4] = {}; // a list of valid moves
    int *new_state;
    int temp;
    Node* new_node;
    int idx, idx_zero;

    for (int i=0; i<*size; i++){
        if (smallDisks->state[i] == 0){
            idx = i;
            idx_zero = largeDisks[i];

            possible_moves[0] = (idx + 1) % *size;
            possible_moves[1] = (idx + *size - 1) % *size;
            possible_moves[2] = (idx + idx_zero) % *size;
            possible_moves[3] = (idx + *size - idx_zero) % *size;

            break;
        }
    }

    for (int i=0; i<4; i++){
        new_state = clone_smallDisks(smallDisks->state, size);

        // performs the swap
        temp = new_state[idx];
        new_state[idx] = new_state[possible_moves[i]];
        new_state[possible_moves[i]] = temp;

        //Check here if the new_state is in the explored_and_frontier
        if(exists(explored_and_frontier, new_state)){
            free(new_state);
            continue;
        }
        
        // Add new_state to frontier
        new_node = create_node(new_state);

        calc_gn(smallDisks, new_node);
        calc_hn(new_node, size, solution);
        calc_fn(new_node);
        new_node->parent = smallDisks;
        add_node(new_node, size, frontier);
        frontier->size++;

        // Add current node to the explored_and_frontier
        insertHashEntry(explored_and_frontier, new_node);
    }
}

/*
Clones the smallDisks.
*/
int* clone_smallDisks(int *smallDisks, int *size){
    int *new_node = malloc((*size)*sizeof(int));
    if (!new_node) {
        fprintf(stderr,"Memory error\n");
        exit(EXIT_FAILURE);
    }
    for (int i=0; i<*size; i++){
        new_node[i] = smallDisks[i];
    }
    return new_node;
}

int main(int argc, char **argv)
{
    int size;
    int *smallDisks;
    int *largeDisks;
    Dictionary explored_and_frontier;
    List *frontier;
    int *solution;

    // Simple error check to ensure correct args
    // does not ensure that the actual arg is valid
    if (argc != 2) {
        fprintf(stderr, "Format must be ./AB <size>\n");
        return EXIT_FAILURE;
    }

    size = atoi(argv[1]);

    smallDisks = malloc(size * sizeof(int));
    if (smallDisks == NULL) {
        fprintf(stderr, "Malloc failed\n");
        return EXIT_FAILURE;
    }

    largeDisks = malloc(size * sizeof(int));
    if (largeDisks == NULL) {
        fprintf(stderr, "Malloc failed\n");
        return EXIT_FAILURE;
    }

    if ((handle_input(smallDisks, largeDisks, size) == 1))
        return EXIT_FAILURE;

    solution = solution_state(smallDisks, &size);
    frontier = makelist();
    explored_and_frontier = create(64, 3, 0.74, size);
    Node *current = create_node(smallDisks);
    current->parent = NULL;
    current->gn = 0;
    calc_hn(current, &size, solution);
    current->fn = current->hn;
    add_node(current, &size, frontier);
    frontier->size = 1;
    insertHashEntry(explored_and_frontier, current);

    // iterates through the frontier till it finds a goal state or the frontier is empty.
    while (frontier->size != 0){
        current = remove_first(frontier);
        //print_node_state(current, &size);

        // Checks if current is the goal state
        if (current->hn <= 0){
            print_solution_path(current, &size);
            break;
        }

        generate_new_states(largeDisks, current, &size, frontier, explored_and_frontier, solution);
        frontier->size--;
    }

    if (frontier->size == 0)
        printf("Failure\n");

    free(solution);
    free(frontier);
    destroy_hashTable(explored_and_frontier);
    free(largeDisks);

    printf("Program Terminated!\n");
    return 0;
}