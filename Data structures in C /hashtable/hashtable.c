
//Name: Brenda Waiya
//Class: CS50 Fall 2023
//Purpose: handles the hashtable data structure 




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "hash.h"
#include "../set/set.h"

/**************** global types ****************/
typedef struct hashtable {
    int num_slots;
    set_t** table;
}hashtable_t;  

/**************** hashtable_new ****************/
/* Create a new (empty) hashtable.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */

hashtable_t* hashtable_new(const int num_slots){
    //handling error cases
    if (num_slots > 0){ // make sure it's not negative
        //allocate memory for the hashtable
        hashtable_t* new = malloc(sizeof(hashtable_t));
        if (new == NULL){
            return NULL;
        }
        else{
            new->num_slots = num_slots;
            //using calloc to initialize all the values to zero
            //contiguous allocation." It is similar to the malloc function
            // but has an additional feature: it initializes the allocated memory to zero (or null) values for each element. 
            //calloc is particularly useful when you need to allocate memory for arrays, matrices, or data 
            //structures that require initialization.
            new->table = calloc(num_slots, sizeof(size_t));
            if (new->table != NULL){
                for (int i = 0; i < num_slots; i++){
                //the slots in the table will have a link to a set
                new->table[i] = set_new();
                }
                return new;
            }
            return NULL;
        }
    }return NULL;
}

/**************** hashtable_insert ****************/
/* Insert item, identified by key (string), into the given hashtable.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 * Notes:
 *   The key string is copied for use by the hashtable; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */

bool hashtable_insert(hashtable_t* ht, const char* key, void* item) {
    // Check for invalid inputs
    if (ht == NULL || key == NULL || item == NULL) {
        return false;
    }

    // Calculate the hash value based on the length of the key 
    //you could also use the hash_jenkins function to calculate hash value :
  
  if(hashtable_find(ht,key) == NULL){
    long hashval = hash_jenkins(key, ht -> num_slots);
    return set_insert(ht->table[hashval], key, item);
  }
  else {
    return false;
  }
    
}

/**************** hashtable_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if hashtable is NULL, key is NULL, or key is not found.
 * Notes:
 *   the hashtable is unchanged by this operation.
 */
void* hashtable_find(hashtable_t* ht, const char* key){
    //
    if (ht == NULL || key == NULL){
        return NULL;
    }
    else{
        //using the hashvalue to find it in the according set
        int hash = hash_jenkins(key, ht->num_slots);
        return set_find(ht->table[hash], key);
    }
}
/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item.
 * 
 * Caller provides:
 *   valid pointer to hashtable, 
 *   FILE open for writing,
 *   itemprint that can print a single (key, item) pair.
 * We print:
 *   nothing, if NULL fp.
 *   "(null)" if NULL ht.
 *   one line per hash slot, with no items, if NULL itemprint.
 *   otherwise, one line per hash slot, listing (key,item) pairs in that slot.
 * Note:
 *   the hashtable and its contents are not changed by this function,
 */
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)){
    //validate user input 
    if (fp != NULL){
        if (ht != NULL){
        for (int i = 0; i < ht->num_slots; i++){
            set_print(ht->table[i], fp, (*itemprint));
            fputs("\n",fp);
        }
    }
      else{
        fputs("(null)",fp);
    }
}
}
/**************** hashtable_iterate ****************/
/* Iterate over all items in the table; in undefined order.
 *
 * Caller provides:
 *   valid pointer to hashtable, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if ht==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, key, item).
 * Notes:
 *   the order in which hashtable items are handled is undefined.
 *   the hashtable and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */

void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)){
    //validate input 
    if (ht != NULL && itemfunc != NULL){
        // call itemfunc with arg, on each item
        for (int i = 0; i < ht->num_slots; i++) {
            set_iterate(ht->table[i], arg, (*itemfunc));
        }
    }
}
/**************** hashtable_delete ****************/
/* Delete hashtable, calling a delete function on each item.
 *
 * Caller provides:
 *   valid hashtable pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if hashtable==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the hashtable itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in hashtable_insert.
 */
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item)){
    if (ht != NULL){
        if (itemdelete != NULL){
            for (int i = 0; i < ht->num_slots; i++){
                set_delete(ht->table[i], itemdelete);
            }
            // need to free both the tables and the hashtable itself
            free(ht->table);
            free(ht);
        }
    }
}