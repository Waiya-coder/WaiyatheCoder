// Name: Brenda Waiya
// Class: CS50 Fall 2023
// Purpose: handles the counter data set 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "file.h"
#include "mem.h"

// opaque to users of the module
// start by making a counternode
// Local type
typedef struct counternode
{
    int key;                // pointer to this data
    struct counternode *next; // pointer to the next item
    int count;                // pointer to the number of times this item has occured
} counternode_t;

// Global type
//  the counter itself
typedef struct counters
{
    struct counternode *head;
} counters_t;

static counternode_t *counternode_new(const int key){
    // allocating memory for a node
  counternode_t *node = malloc(sizeof(counternode_t));
  
  if (node == NULL)
  {
    return NULL;
  }
  // if no error initialize a new node
  else
  {
   
    node->key = key;
    node->count = 1;
    node->next = NULL;
    return node;
  }
    
}

/**************** functions ****************/

/**************** FUNCTION ****************/
/* Create a new (empty) counter structure.

 *
 * We return:
 *   pointer to a new counterset; NULL if error.
 * We guarantee:
 *   counterset is intialized empty.
 * Caller is responsible for:
 *   later calling counters_delete();
 */
counters_t *counters_new(void)
{
    counters_t *newcounter = malloc(sizeof(counters_t));
    if (newcounter == NULL)
    {
        return NULL;
    }
    else
    {
        // initialize a new counter
        newcounter->head = NULL;
        return newcounter;
    }
}

/**************** counters_add ****************/
/* Increment the counter indicated by key.
 *
 * Caller provides:
 *   valid pointer to counterset, and key(must be >= 0)
 * We return:
 *   the new value of the counter related to the indicated key.
 *   0 on error (if ctrs is NULL or key is negative)
 * We guarantee:
 *   the counter's value will be >= 1, on successful return.
 * We do:
 *  ignore if ctrs is NULL or key is negative.
 *  if the key does not yet exist, create a counter for it and initialize to 1.
 *  if the key does exist, increment its counter by 1.
 */
int counters_add(counters_t *ctrs, const int key)
{

    if (key > 0 || ctrs != NULL)
    {// handle the situation where the crts is empty
        if (ctrs->head == NULL)
        {
            ctrs->head = counternode_new(key);
        }
        //  //if the key is there, only increment the counter for the key.
        else
        {
            for (counternode_t *node = ctrs->head; node != NULL; node = node->next)
            {
                if (key == node->key)
                {
                    node->count++;
                    return node->count;
                }
            }
             
                counternode_t *new_node = counternode_new(key);
                counternode_t* prev = ctrs->head;
                ctrs-> head = new_node;
                new_node->next= prev;
                return 1;
            }
        }
        return 0;
}
    
    


/**************** counters_get ****************/
/* Return current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset, and key(must be >= 0)
 * We return:
 *   current value of counter associte with the given key, if present,
 *   0 if ctrs is NULL or if key is not found.
 * Note:
 *   counterset is unchanged as a result of this call.
 */
int counters_get(counters_t *ctrs, const int key)
{
    if (key >= 0 || ctrs != NULL)
    {

        // loop through the counterset
        for (counternode_t *node = ctrs->head; node != NULL; node = node->next)
        {
            // if the key exists in the counterset, return the count of that node
            if (node->key == key)
            {
                return node->count;
            }
        }
    }
    return 0;
}
/**************** counters_set ****************/
/* Set the current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset,
 *   key(must be >= 0),
 *   counter value(must be >= 0).
 * We return:
 *   false if ctrs is NULL, if key < 0 or count < 0, or if out of memory.
 *   otherwise returns true.
 * We do:
 *   If the key does not yet exist, create a counter for it and initialize to
 *   the given value.
 *   If the key does exist, update its counter value to the given value.
 */

bool counters_set(counters_t *ctrs, const int key, const int count)
{
    // dealing with invalid user inputs
    if (key < 0 || ctrs == NULL || count < 0)
    {
        return false; // because it's a boolean function
    }
    else
    {
        counternode_t *last_node; // creating variable to save the last node to add a potential new node to the end of the set
        // looping through each counternode
        for (counternode_t *node = ctrs->head; node != NULL; node = node->next)
        {
            // if the key exists, update the count and return the boolean as true because this is a boolean function
            if (key == node->key)
            {
                node->count = count;
                return true;
            }
            last_node = node;                           // we've basically gone through the entire counter set and we are at the last counternode
        }                                               // exit for-loop
        counternode_t *new_node = counternode_new(key); // make a new counternode
        last_node->next = new_node;                     // use the variable created earlier on to add the new node to the end of the set
        new_node->count = count;                        // set the count as count
        return true;
    }
}

/**************** counters_print ****************/
/* Print all counters; provide the output file.
 *
 * Caller provides:
 *   valid pointer to counterset,
 *   FILE open for writing.
 * We print:
 *   Nothing if NULL fp.
 *   "(null)" if NULL ctrs.
 *   otherwise, comma=separated list of key=counter pairs, all in {brackets}.
 */
void counters_print(counters_t *ctrs, FILE *fp)
{
    // handle invalid user inputs
    if (ctrs != NULL)
    {

        if (fp == NULL)
        {
            fputs("(null)\n", fp);
            //return 0;
        }
        // where the counter is actually empty
        if (ctrs->head == NULL)
        {
            fputs("null\n", fp);
        }
        // where the counterset is not empty
        else
        {
            fputs("{", fp);
            for (counternode_t *node = ctrs->head; node != NULL; node = node->next)
            {
                fprintf(fp, "[%d:%d],", node->key, node->count);
            }
            fputs("}", fp);
        }
    }
}

/**************** counters_iterate ****************/
/* Iterate over all counters in the set.
 *
 * Caller provides:
 *   valid pointer to counterset,
 *   arbitrary void*arg,
 *   valid pointer to itemfunc that can handle one item.
 * We do:
 *   nothing, if ctrs==NULL or itemfunc==NULL.
 *   otherwise, call itemfunc once for each item, with (arg, key, count).
 * Note:
 *   the order in which items are handled is undefined.
 *   the counterset is unchanged by this operation.
 */
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count))
{
    if (ctrs != NULL && arg != NULL && itemfunc != NULL)
    {

        for (counternode_t *node = ctrs->head; node != NULL; node = node->next)
        {
            (*itemfunc)(arg, node->key, node->count);
        }
    }
}
    /**************** counters_delete ****************/
    /* Delete the whole counterset.
     *
     * Caller provides:
     *   a valid pointer to counterset.
     * We do:
     *   we ignore NULL ctrs.
     *   we free all memory we allocate for this counterset.
     */
    void counters_delete(counters_t * ctrs)
    {
        if (ctrs != NULL)
        {
            // we need to save the next node within the for-loop to not lose the next pointer when the memory is freed.
            // because of that, the third part in the for loop is not there to move the pointer within the for-loop
            for (counternode_t *node = ctrs->head; node != NULL;)
            {
                counternode_t *next = node->next;
                free(node);
                node = next;
            }
            mem_free(ctrs);
        }
    }
