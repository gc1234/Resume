//This code is based from: 
//https://github.com/skorks/c-linked-list

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "linked_list.h"

Node * create_node(int *state){
  Node * newNode = malloc(sizeof(Node));
  if (!newNode) {
      fprintf(stderr, "Malloc failed\n");
      exit(EXIT_FAILURE);
    }
  newNode->state = state;
  return newNode;
}

List * makelist(){
  List * list = malloc(sizeof(List));
  if (!list) {
    fprintf(stderr, "Malloc failed\n");
    exit(EXIT_FAILURE);
    }
  list->head = NULL;
  return list;
}

/*
 * For testing purposes
 */
void display(Node *node, int *size, List * list) {
  Node * current = list->head;
  printf("Possible moves: ");
  printf("[");
  for (int i=0; i<*size; i++)
      printf("%d ", node->state[i]);
  printf("], ");
  
  for(; current != NULL; current = current->next) {
    printf("[");
    for (int i=0; i<*size; i++)
      printf("%d ", current->state[i]);
    printf("], ");
  }
  printf("\n");
}
                                

void print_solution_path(Node *node, int *size){
  Node * reversed = NULL;
  Node * current = node;
  Node * temp = NULL;
  while(current != NULL){
    temp = current;
    current = current->parent;
    temp->parent = reversed;
    reversed = temp;
  }
  node = reversed;
  print_path_to_goal(node, size);
}

void destroy(List * list){
  Node * current = list->head;
  Node * next = current;
  while(current != NULL){
    next = current->next;
    free(current->state);
    free(current);
    current = next;
  }
  free(list);
}

bool compare_states(Node *state1, Node *state2, int *size){
  for (int i=0; i<*size; i++){
    if (state1->state[i] != state2->state[i])
      return false;
  }
  return true;
}

void print_node_state(Node *node, int *size){
  for (int i=0; i<*size; i++){
    printf("%d ", node->state[i]);
  }
  printf("\n");
}

Node* remove_first(List *list){
  Node * current = list->head;
  list->head = list->head->next;            
  current->next = NULL;
  return current;
}

void add_node(Node *node, int *size, List * list){
  Node * current = NULL;
  if((list->head == NULL) || (node->fn < list->head->fn) || 
  ((list->head->fn == node->fn) && (node->gn < list->head->gn))){
    node->next = list->head;
    list->head = node;
  }else{
    current = list->head; 
    while((current->next != NULL) && (node->fn >= current->next->fn) && ((current->next->fn != node->fn) && (node->gn >= current->next->gn))){
      current = current->next;
    }
    node->next = current->next;
    current->next = node;
    list->size++;

  }
}

void print_path_to_goal(Node* node, int *size){
    printf("The solution path to the goal:\n");
    for (Node *p = node; p!=NULL; p=p->parent){
      print_node_state(p, size);
    }
}

bool node_exists(Node *node, List *list, int *size){
  Node *current = list->head; 
    while(current != NULL){
      if (compare_states(current, node, size))
        return true;
      current = current->next;
    }
    return false;
}