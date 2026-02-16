//Name: Brenda Waiya
//Class: CS50 Fall 2023
//Purpose: Handles the set module


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "../lib/mem.h"

//local variables starting with the node itself
typedef struct setnode{
  char* key; //the key
  struct setnode* next; //pointer to the next node
  void* item; //data in the node
} setnode_t;  // will be used to create instances of nodes

typedef struct set{
  struct setnode* head; // initialize a node to be the head of the set
} set_t; //type data will be used to create instances of sets

//FUNCTION TO CREATE A NEW SETNODE/ local function 
static setnode_t* setnode_new( const char* key, void*item){
  //allocating memory for a node
  setnode_t* node = malloc(sizeof (setnode_t));
  //allocate memory to a copy of the key : length of the key and the +1 is for special character in C strings called the null terminator ('\0') and then the size of each char in the string 
  char* key_copy = malloc((strlen(key)+1 ) * sizeof(char));
  //handle error cases
  if (node ==NULL || key_copy==NULL){
    return NULL;
  }
  //if no error initialize a new node 
  else{
  strcpy(key_copy,key);
   node->key = key_copy;
   node-> item = item;
   node-> next = NULL;
   return node;

}
}
//FUNCTION TO CREATE A NEW SET 
set_t* set_new(void){
  //allocate memory for a new set
  set_t* set = malloc(sizeof(set_t));
  //handle errors
  if(set== NULL){ 
    return NULL;
  }
  //if no error initialize new set 
  else{ 
    set-> head = NULL;
    return set;


  }
}
//function to insert a new node into the set
/**************** set_insert ****************/
/* Insert item, identified by a key (string), into the given set.

 *
 * Caller provides:
 *   valid set pointer, valid string pointer, and pointer to item.
 * We return:
 *  false if key exists, any parameter is NULL, or error;
 *  true iff new item was inserted.
 * Caller is responsible for:
 *   later calling set_delete to free the memory used by key strings.
 * Notes:
 *   The key string is copied for use by the set; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */

bool set_insert(set_t* set, const char* key, void* item){//Handle all error cases an return a false boolean if anything is amiss
    
    if(set_find(set,key) == NULL){
      if(set!= NULL && key != NULL && item != NULL){
        setnode_t* tempset = setnode_new(key,item);
        if(tempset != NULL){
          tempset-> next = set->head;
          set-> head = tempset;
          return true;
        }
      }
    }
    return false;
      }
 /**************** set_iterate ****************/
/* Iterate over the set, calling a function on each item.
 * 
 * Caller provides:
 *   valid set pointer,
 *   arbitrary argument (pointer) that is passed-through to itemfunc,
 *   valid pointer to function that handles one item.
 * We do:
 *   nothing, if set==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc on each item, with (arg, key, item).
 * Notes:
 *   the order in which set items are handled is undefined.
 *   the set and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */   
      
    
      



 

//function to iterate through the set
void set_iterate(set_t* set, void* arg,
                 void (*itemfunc)(void* arg, const char* key, void* item)){
                  if(set != NULL && itemfunc != NULL){
                    
  //loop through the set , calling the itemfunc on each  on each item
  for ( setnode_t *node = set-> head; node != NULL;node= node-> next){
    (*itemfunc)(arg, node->key, node->item);
  }
                 }
                 }
                 

/* Delete set, calling a delete function on each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if set==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the set itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in set_insert.
 */

//function to delete a set or a setnode 
void set_delete(set_t* set, void (*itemdelete)(void* item)) {
  if (set != NULL && itemdelete != NULL) {
        setnode_t* node = set->head;
        while (node != NULL) {
            setnode_t* next = node->next;

            // Delete the item associated with the node
            if (node->item != NULL) {
                itemdelete(node->item);
            }

            // Free the key (assuming key is a dynamically allocated string)
            if (node->key != NULL) {
                mem_free(node->key);
            }

            // Free the node itself
            mem_free(node);

            node = next; // Move to the next node
        }
    }

    // Finally, free the set structure itself

  mem_free(set);

}
    


//function to print a set
/**************** set_print ****************/
/* Print the whole set; provide the output file and func to print each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   FILE open for writing,
 *   valid pointer to function that prints one item.
 * We print:
 *   nothing if NULL fp. Print (null) if NULL set.
 *   print a set with no items if NULL itemprint. 
 *  otherwise, 
 *   print a comma-separated list of items surrounded by {brackets}.
 * Notes:
 *   The set and its contents are not changed.
 *   The 'itemprint' function is responsible for printing (key,item).
 */
void set_print(set_t* set, FILE* fp, 
    void(*itemprint)(FILE* fp, const char* key, void* item) ){
          if (set != NULL || fp != NULL) {
          
          if (itemprint == NULL){
            fputs("{}\n", fp);

          }
            // where the set is actually empty 
          if ( set-> head == NULL){
            fputs("null\n", fp);
          }
          //where the set is not empty 
          else{
            fputs("{",fp );
            for (setnode_t* node = set-> head; node != NULL; node =node->next){
                    (*itemprint)(fp, node-> key , node -> item);
                  

            }  
            fputs( "}", fp ) ;
                 
          }
          }  
          }

    


//function to find a node in a set 

/**************** set_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid set pointer, valid string pointer.
 * We return:
 *   a pointer to the desired item, if found; 
 *   NULL if set is NULL, key is NULL, or key is not found.
 * Notes:
 *   The item is *not* removed from the set.
 *   Thus, the caller should *not* free the pointer that is returned.
 */
void* set_find(set_t* set, const char* key){
  if ( set == NULL|| key ==NULL) {
    return false ;
  }
  else {
    for (setnode_t *node = set-> head ; node!= NULL; node = node -> next){
            if(strcmp ((node -> key), key) == 0) {
              return node-> item;
            }
            else{
              return NULL;
              printf( "Key not found \n");
              
            }

    }
  }
return 0;
}
