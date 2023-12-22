// Name: Brenda Waiya
// Class: CS50 Fall 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "word.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

void index_item_delete(void* item);
//void itemprint(void* arg, const char* key, void* item);
void counters_itemprint(void* arg, const int key, const int number);


/**************** functions ****************/

//making a new index. see index.h for more information
index_t*
index_new(const int numslots){

    

    //give it a hashtable
   index_t* index = hashtable_new(numslots);

    return index;
}

//deleting index. see index.h for more information
void index_delete(index_t* index){
    if (index != NULL){
        hashtable_delete(index, index_item_delete);
    }
    
}

//local helper function for index_delete
void index_item_delete(void* item){
    counters_delete(item);
}

//see index.h for more information.
counters_t* index_find(index_t* index, void* key){
    
    if (index != NULL && key != NULL){
        return hashtable_find(index, key);
    }
    else{
        return NULL;
    }

}

//see index.h for more information
void index_insert(index_t* index, void* key, void* item){
    if (index != NULL && key != NULL){
        hashtable_insert(index, key, item);
       
    }
}


// 
//local function to help index_save
void itemprint(void *fp, const char *key, void *item) {

    fflush(stdout);



    if (fp == NULL) {
        fprintf(stderr, "Error: File pointer is NULL\n");
        return;
    }
     // Need to now iterate through the counters for that word
    counters_t *counters = (counters_t*)item;
    // Print the word first. '\n' is not included because more information has to come on that line.
    fprintf(fp, "%s ", key);

   

    if (counters != NULL) {
        counters_iterate(counters, fp, counters_itemprint);
    } else {
        fprintf(fp, "Counters are NULL\n");
    }

    fprintf(fp, "\n");
}


//local function to help index_save by helping itemprint
void counters_itemprint(void* arg, const int key, const int number){
    FILE* fp;
    fp = arg;
    //printing in the right format now
    fprintf(fp, "%d %d ", key, number);
    
}

//see index.h for more information
void index_save(char* filename, index_t* index){
    FILE* fp; 
    fp = fopen(filename, "w");
    if (fp == NULL){
        fprintf(stderr, "File cannot be opened");
        exit(7);
    }
    hashtable_iterate(index, fp, itemprint);

   
   // fflush(stdout);
   
    
    
    fclose(fp);
}

//more information on index.h
void index_load(char* filename, index_t* index){
    FILE* fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Failed to open file");
        return;
    }
 
    // Initialize variables
    char* word = NULL;
    int ID;
    int number;

    // Loop through the words in the file
    while ((word = file_readWord(fp)) != NULL) {
        // Create a counters for the word
        counters_t* counters = counters_new();
        if (counters!= NULL){
                // Read pairs of integers for the word until the format doesn't match
        while (fscanf(fp, "%d %d", &ID, &number) == 2) {
            counters_set(counters, ID, number);
        }

        // Insert the word and its associated counters into the index
        index_insert(index, word, counters);

        // Free the memory for the word
        free(word);
    }
    else{
        fprintf(stderr, "counters failed to create");
    }

    
}
        }


        


#ifdef UNIT_TEST




#endif // UNIT_TEST