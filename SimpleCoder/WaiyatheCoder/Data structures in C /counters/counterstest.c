
 //Name: Brenda Waiya
//Class: CS50 Fall 2023
//Purpose: Test the set module through normal and edge cases
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
// #include "file.h"


static void itemfunc (void* arg, const int key, int count);
int main(){
    //create an empty counter 
    counters_t* counter1 ;
    counter1 = counters_new();
     //initialize an iterator 
     int count = 0;
   
    ////ensure the counter  is made 
if (counter1 == NULL){
        printf( "Error, counter1 has not been created successfully \n");
        return 1;

}
else{
    printf("counter1 has been created successfully\n");
}

    //add stuff to the counter and check whether the add function works correctly 
    counters_add(counter1, 100);
    counters_add(counter1, 110);
    counters_add(counter1, 120);
    counters_add(counter1, 130);
    counters_add(counter1, 400);
    printf("Should print 100,110,120,130,400\n");
    counters_print(counter1,stdout);
//ensure that the count is updating correctly
    counters_add(counter1,100);
    counters_add(counter1, 120);
    printf("should print one 100s and one 120s\n");
    counters_print(counter1,stdout);
    //ensure that the set function is working 
    counters_set(counter1, 120, 5);
    counters_set(counter1, 200, 2);
    counters_set(counter1, 400, 3);
    counters_set(counter1,5,3);
    printf("should print one 100, one 110, five 120s, one 130s, three 400s, two 200s\n");
    counters_print(counter1, stdout);

    //ensure that the get function is working 
    //if given a key, you get the associated count value
    int value;
    value = counters_get(counter1, 120);
    printf("\nShould print 5\n");
    printf("%d\n", value);  

    //iterating through counter to count objects
    count = 0;
    counters_iterate(counter1, &count, itemfunc);

    //deleting to make sure that there are no memory leaks
    counters_delete(counter1);
    
}

static void itemfunc(void* arg, const int key, const int count){
    int* items = arg;
    if (items != NULL){
        (*items)++;
    }
}

   


    

