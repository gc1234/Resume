#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hash.h"
#include "linked_list.h"

#define MULTIPLIER (32)

//hash function from www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29
static unsigned long hash_function(int *state, int size)
{
    char* s = convert_state_to_chars(state, size);
    unsigned const char *us;
    unsigned long h;
    
    h = 0;
    
    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    free(s);
    return h;
}

void grow(Dictionary table)
{
    Dictionary newTable = create(table->table_maxsize * table->table_delta_capacity, table->table_delta_capacity, table->table_load_threshold, table->size);
    
    // swaps the new enteries of the previous dictionary with the new dictionary and frees the memory
    for (int i=0; i<table->table_maxsize; i++){
        if (table->data[i]!=NULL){
            insertHashEntry(newTable, table->data[i]);
        }
    }
    //swaps the newTable->data pointer with table->data and frees the memory.
    free(table->data);
    table->data = newTable->data;
    table->table_maxsize = newTable->table_maxsize;
    free(newTable);

    return;
}

Dictionary create(int initial_capacity, int delta_capacity, float load_threshold, int size)
{
    Dictionary newHashTable=malloc(sizeof(hashTable));
    
    if (!newHashTable){
        fprintf(stderr,"Error Allocating Memory\n");
        exit(EXIT_FAILURE);
    }
    newHashTable->size = size;
    newHashTable->table_maxsize = initial_capacity;
    newHashTable->table_cursize = 0;
    newHashTable->table_delta_capacity = delta_capacity;
    newHashTable->table_load_threshold = load_threshold;
    newHashTable->data = malloc(initial_capacity * sizeof(Node *));
    if (!(newHashTable->data)){
        fprintf(stderr,"Error Allocating Memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i=0; i<initial_capacity; i++){
        newHashTable->data[i]=NULL;
    }
    return newHashTable;
}

void destroy_hashTable(Dictionary table)
{
    for (int i=0; i<table->table_maxsize && table->data[i]!=NULL;i++){
        free(table->data[i]->state);
        free(table->data[i]);
    }
    free(table->data);
    free(table);
    return;
}

int exists(Dictionary table, int *index)
{
    int i = 0;
    unsigned long hash = (hash_function(index, table->size) + i) % table->table_maxsize;
    
    while (table->data[hash] != NULL){
        if (compareStates(table->data[hash]->state, index, table->size))
            return 1;
        i++;
        hash = (hash_function(index, table->size) + i) % table->table_maxsize;
    }
    return 0;
}

void insertHashEntry(Dictionary table, Node *index)
{
    int i = 0;
    float full_table = (float) table->table_cursize / table->table_maxsize;
    unsigned long ind = (hash_function(index->state, table->size) + i) % table->table_maxsize;
    
    //check if capacity was reached, and grow table if needed
    if (full_table > table->table_load_threshold)
        grow(table);

    //inserts at a NULL hash position in the table or if a condition within is met
    for (i = 0; table->data[ind] != NULL;){
        i++;
        ind = (hash_function(index->state, table->size) + i) % table->table_maxsize;
    }

    table->data[ind] = index;
    table->table_cursize++;
    return;
    
}

bool compareStates(int *state1, int *state2, int size){
    for (int i=0; i<size; i++){
        if (state1[i] != state2[i])
            return false;
    }
    return true;
}

char* convert_state_to_chars(int *state, int size){
    char *string = malloc((size+1)*sizeof(char));
    if (!(string)){
        fprintf(stderr,"Error Allocating Memory\n");
        exit(EXIT_FAILURE);
    }
    int i;
    for (i=0; i<size; i++){
        string[i] = '0' + state[i];
    }
    string[i] = '\0';
    return string;
}