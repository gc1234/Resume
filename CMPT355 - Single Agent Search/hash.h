#ifndef hashP_H
#define hashP_H

#include "linked_list.h"

typedef struct {
    int table_maxsize;
    int table_cursize;
    int table_delta_capacity;
    float table_load_threshold; //create new table at this %
    Node ** data;
    int size;
} hashTable;

typedef hashTable * Dictionary;

//static unsigned long hash_function(int *state, int size);
/*
 creates a hash value of a string.
 Pre: takes a state
 Post: None
 Returns: A hash value as unsigned long int
 */

void grow(Dictionary table);
/*
 Creates a new hashtable bigger than the current one.
 Pre: takes a tashtable as a value
 Post: destroys the old hashtable and creates a newone in its pass and swaps all the values from the old one to the new one.
 Returns: None.
 */

Dictionary create(int initial_capacity, int delta_capacity, float load_threshold, int size);
/*
 * Create a new Dictionary and return a handle to the Dictionary.
 *
 * Pre: initial_capacity >= 0, delta_capacity >= 0 float_threshold >0 float_threshold<=1;
 *
 * Post: returns a handle to an empty Dictionary that has an initial capacity of
 *      initial_capacity items, and that is expanded by delta_capacity
 *      items when the Dictionary fills above the load_threshold
 * We need it to match for our test program).
 *      returns NULL on failure
 *
 * Side Effects: allocates storage.
 */

void destroy_hashTable(Dictionary table);
/*
 * Destroy an existing Dictionary with handle table.
 *
 * Pre: table is a valid handle
 * Post: none
 *
 * Side Effects: all memory allcated for the map is freed, including memory
 * associated with the actual data in an item.
 */

int exists(Dictionary table, int * index);
/*
 * Test if word index exists in the table with matching definition
 *
 * Pre: table is a valid handle
 * Post: returns 1 if index exists in the table, that is if a table contains
 *       a pair <index,def> for some value def, 0 otherwise
 *
 */

void insertHashEntry(Dictionary table, Node * index);
/*
 * Add an new entry to the table.
 *
 * Pre: table is a valid handle,
 *      index and value not NULL
 *
 * Post: a new entry is added to the hash table
 *
 * Hash table being extended if it is full.
 *
 */
bool compareStates(int *state1, int *state2, int size);
/*
 * Compares state1 and state2 to see if they are the same.
 * Returns true/false
 */
char* convert_state_to_chars(int *state, int size);
/*
 * Converts a state into a string.
 * 
 * Returns a string.
 */

#endif