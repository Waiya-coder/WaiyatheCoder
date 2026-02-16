// Name: Brenda Waiya
// Class: CS50 Fall 2023
#include <stdio.h>
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/**
 * creates a new index structure
 * Allocate memory for the index, and the hashtable for the index. 
 * 
 * index_delete must free both allocations of memory 
 */
index_t* index_new(const int numslots);

/**************** index_delete ****************/
/**
 * deletes an index structure
 * first delete the hashtable and the counters in it
 * and then delete the index 
 */
void index_delete(index_t* index);

/**************** index_save ****************/
/**
 * saves an index structure to the filename given. 
 * Uses two local helper functions. 
 * 
 * It iterates through the hashtable first. However, as it iterates through,
 * it calls another helper function before it moves onto the next word
 * so that it can print the (docID, number) for the appropriate word. 
 * 
 */
void index_save(char* filename, index_t* index);

/**************** index_find ****************/
/**
 * accesses the hashtable in the index struct
 * given a key it will call hashtable_find
 * 
 * if caller doens't provide a valid index or key, it will exit null
 */
counters_t* index_find(index_t* index, void* key);


/**************** index_find ****************/
/**
 * will insert the key and item into the hashtable
 * mostly leverages hashtable_insert
 * 
 * On error, will return null.
 */
void index_insert(index_t* index, void* key, void* item);

/**************** index_load ****************/
/**
 * will read into a file that is produced by indexer
 * then load the information into an index struct
 * 
 * More specicially, will read from filename and then return 
 * a pointer to the index struct that it made. 
 * 
 * the caller should validate the parameters prior to
 * passing into this function. 
 */
void index_load(char* filename, index_t* index);