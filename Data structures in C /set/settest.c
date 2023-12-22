//Name: Brenda Waiya
//Class: CS50 Fall 2023
//Purpose: Test the set module through normal and edge cases



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "../lib/file.h"


static void itemfunc (void* arg, const char* key, void* item);
static void itemdelete(void* item) ;
static void printname (FILE* fp, const char* key, void* item);

int main() {
    

        set_t* set1 = NULL; //initialize a variable of the type set_t and make sure its empty
        set1 = set_new(); //create an empty set

//initialize a variable that counts the number of non-null items in the set
int count = 0;
//ensure the set is made 
if (set1 == NULL){
        printf( "Error, set1 has not been created successfully \n");
        return 1;

}
else{
    printf("Set1 has been created successfully\n");
}
// assign values to values to the keys and the items
//allocate memory to key-pointers and item pointers that you plan to add to the set
    char* key_pointer1 =  NULL;
    char* key_pointer2 =  NULL;
    char* key_pointer3 =  NULL;
    char * key_pointer4 = NULL;
   
//create an empty set

    key_pointer1 = "Masterpieces of African Literature";
    key_pointer2 = "Software Development" ;
    key_pointer3 = "Neuroscience";
    key_pointer4 = "Piano";
    int item1= 100;
    int item2= 98;
   int item3 = 95;
  int item4 = 96;
    //make sure to print an empty set because you haven't inserted the values into the set
    printf("Set empty \n");
    set_print (set1,stdout,printname);

//insert them into the set 
 set_insert( set1, key_pointer1, (&item1));
 set_insert( set1, key_pointer2, (&item2));
 set_insert( set1, key_pointer3, &item3);
 set_insert( set1, key_pointer4, &item4);
 set_insert( set1, key_pointer4, &item4);

 //print out the full set now 
 printf("It should print out [100,98,95,96] and we have: \n");
 set_print(set1, stdout, printname);

 //try adding duplicates 

 set_insert(set1, key_pointer1,&item4);
 printf(" \n Set should not contain duplicates. \n");
 set_print(set1, stdout, printname);

 //it should count the number of non-null items in the set
 set_iterate(set1, &count , itemfunc);
 set_delete(set1, itemdelete);
 
}
//define the functions we've been using throughout the code
// print a name, in quotes.
void printname(FILE* fp, const char* key, void* item)
{
  int* name = item; 
  if (name == NULL) {
    fprintf(fp, "(null) \n");
  }
  else {
    fprintf(fp, "[%d],", *name); 
  }
}


// delete a name 
void itemdelete(void* item)
{
  if (item != NULL) {
    item = NULL;
  }
}

//counts the number of non-null items in the set and basically updates set_count 
static void itemfunc(void* arg, const char* key, void* item)
{
  int* items = arg;

  //the iterable function that calls item func already loops through the set, so no need to loop again here
  if (items != NULL && item != NULL && key != NULL)
    (*items)++; 
}



