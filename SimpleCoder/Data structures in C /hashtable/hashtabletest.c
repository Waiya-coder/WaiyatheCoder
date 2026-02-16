/* CS50
 * Brenda Waiya 
 *Lab 3, testing file to test out cases for the hashtable module
 *10/6/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "hash.h"
#include "../lib/file.h"

//local functions
static void itemfunc (void* arg, const char* key, void* item);
static void itemdelete(void* item) ;
static void printname (FILE* fp, const char* key, void* item);

int main(){

    //Initializing hashtable. The following inserts and deletes execute without memory leak.
    hashtable_t* hashtable1 = hashtable_new(10);

    //allocating memory for the items 
    int* item1 = malloc(sizeof(int*));
    int* item2 = malloc(sizeof(int*));
    int* item3 = malloc(sizeof(int*));

    //assigning values
    *item1 = 10;
    *item2 = 9;
    *item3 = 8;

    //assigning values
    const char* key1 = "one";
    const char* key2 = "two";
    const char* key3 = "three";

    
    //variable to use for the iteration function
    int hash_count = 0;

    //checking if the hashtable was made successfully
    if (hashtable1 == NULL) {
        fprintf(stderr, "Hashtable1 has not been created succesfully\n");
    return 1;
    }
    else{
        printf("successfully created hashtable1\n");
    }

    //inserting three items into the hashtable
    hashtable_insert(hashtable1, key1, item1);
    hashtable_insert(hashtable1, key2, item2);
    hashtable_insert(hashtable1, key3, item3);

    printf("Should print a hashtable with {one-10, two-9, and three-8}\n");
    hashtable_print(hashtable1, stdout, printname);


    hashtable_iterate(hashtable1, &hash_count, itemfunc);
    printf("The following number will be 3 if the iteration function is working correctly: %d\n", hash_count);

    hashtable_delete(hashtable1, itemdelete);

}
//prtins a name
static void printname(FILE* fp, const char* key, void* item) {
    char* name = item; 
  if (name == NULL) {
    fprintf(fp, "(null)");
  }
  else {
    fprintf(fp, "[%s,%d],", key, (*name)); 
  }
}

//deletes a name
static void itemdelete(void* item){
    if (item != NULL) {
        free(item);   
    }
}

//updates the count when it encounters a new item
static void itemfunc(void* arg, const char* key, void* item){
    int* items = arg;

    if (items != NULL && item != NULL && key != NULL)
    (*items)++; 
}

