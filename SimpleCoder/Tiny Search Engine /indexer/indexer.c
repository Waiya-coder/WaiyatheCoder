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
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "indexer.h"


// function prototype     

index_t* index_build(char *directory,index_t *index );
void indexPage(webpage_t *webpage, index_t *index, int ID);
void parseArgs( char* directory, char * filename );

int main(int argc, char *argv[])
{

    // parsing and validating arguments.
    // validating the number of arguments

    if (argc != 3)
    {
        fprintf(stderr, "See correct usage again.\n");
        exit (1);
    }

    // checking that they aren't null
    for (int i = 0; i < 3; i++)
    {
        if (argv[i] == NULL)
        {
            fprintf(stderr, "Not all arguments are valid.\n");
            return 1;
        }
    }

    // initializing the arguments
    // directory
    char *directory = argv[1];
    //strcpy(directory, );
    // filename
    char *filename = argv[2];
   // strcpy(filename, ;



    // initializing data structure
    index_t *new_index = index_new(500); 

    if (!pagedir_validate(directory))
    {   fprintf(stderr, "Invalid directory");
        exit(3);
        
    }
    index_build(directory, new_index);
    
    index_save(filename, new_index);

    // free all memory allocated
    index_delete(new_index);
    //free(directory);
    //free(filename);
    return 0;
}

// Function to parse command-line arguments and validate inputs
void parseArgs(char* directory, char* filename) {
    // Check if a filename is provided
    if (filename != NULL) {
        // Attempt to open the file for writing
        FILE* pointer = fopen(filename, "w");

        // Check if the file could not be opened
        if (pointer == NULL) {
            // Print an error message to stderr
            fprintf(stderr, "File is not writable");
        } else {
            // Close the file if it was successfully opened
            fclose(pointer);
        }
    } else {
        // If no filename is provided, print an error message and exit with code 2
        fprintf(stderr, "File index is null");
        exit(2);
    }

    // Validate the page directory
    if (!pagedir_validate(directory)) {
        // Print an error message to stderr if the directory does not match the crawler's directory
        fprintf(stderr, "Page directory does not match the directory produced by the crawler\n");
        exit(3);
    }
}


index_t* index_build(char *directory, index_t* index)
{
    

    //index_t* index = index_new(500);
    
    if (directory == NULL){
        mem_free(index);
        return NULL;
    }
        // Initial doc ID
        int ID = 1;
       
       webpage_t *webpage = pagedir_load(directory, ID);

    // first while loop is to loop through the files in the directory
    while (webpage!= NULL){
        indexPage(webpage, index, ID);
        ID++;
        webpage_delete(webpage);
        webpage = pagedir_load(directory, ID);
    }
    return index;
}

        

        // the rest of the file is all html, so use file_readFile
        //char *html = file_readFile(fp);
void indexPage(webpage_t *webpage, index_t *index, int ID){
        if (webpage == NULL|| index== NULL|| ID<0){
            fprintf(stderr, "Invalid error\n");
            exit(5);
        }

        // save the information to a webpage struct
        //
        // initializing
        int pos = 0;
        char *word;

        // this nested loop is to loop through the words in the file
        while ((word = webpage_getNextWord(webpage, &pos)) != NULL)
        {
            if (strlen(word) >= 3)
            {

               char* normWord= normalize_word(word);
                if (normWord != NULL){
                 counters_t *counters;

                        
                    // if it's the first time that the word is seen
                    if ((counters = index_find(index, normWord))== NULL)
                    {
                        // add a new counters for that word.
                        counters_t* new_counters = counters_new();
                    
                        // and add a new entry in the index
                        index_insert(index,normWord, new_counters);
                        // index_ins:qsort:qsort(index, normWord, new_counters);
                    
                    }
                    // if it's not the first time,
                    else {
                        // retrieve the pointer to the counters, and use the doc ID to increment the count
                        counters_add(counters, ID);
                      
                    }
                    
                }
            }
            free(word);
            
        }

    

       



}
        