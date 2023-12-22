// Name: Brenda Waiya
// Class: CS50 Fall 2023

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/webpage.h"

int main (int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "See appropriate usage again\n");
        return 1; 
    }
 

    //initializing data structures and variables
    FILE* fp = fopen(argv[1], "r");
   // int slots = file_numLines(fp);
    fclose(fp);

    index_t* index = index_new(500);

    index_load(argv[1], index);
    index_save(argv[2], index);

    index_delete(index);

    return 0;
}