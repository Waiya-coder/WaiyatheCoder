
// Name: Brenda Waiya
// Class: CS50 Fall 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"


// Function to initialize the page directory and create the .crawler file
bool pagedir_init(const char *pageDirectory)
{
    // allocate memory to create a filename to test if that opens and is writable
    FILE *fp = NULL;
    char *filename = malloc(strlen(pageDirectory) + 20);

    sprintf(filename, "%s/.crawler", pageDirectory);

    // if it's null, free the memory and return false.
    if ((fp = fopen(filename, "w")) == NULL)
    {
        free(filename);
        return false;
    }
    // if it's successfull, close the file, free the memory and return true
    else
    {
        fclose(fp);
        free(filename);
        return true;
    }
}

// Function to save a webpage to the page directory
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID)
{
 
    //char *pathname= (char *)malloc(strlen(pageDirectory) + sizeof(docID) + 10);
    // initializing file pointer and filename
    FILE *fp = NULL;
    char *filename = (char *)malloc(strlen(pageDirectory) + sizeof(docID) + 10);

    sprintf(filename, "%s/%d", pageDirectory, docID);

    // if it can't open it, log the error
    if ((fp = fopen(filename, "w")) == NULL)
    {
        fprintf(stderr, "There was an error in writing the file\n");
    }
    // if not, write in the file according to format
    else
    {
        
        fprintf(fp, "%s\n", webpage_getURL(page));
        fprintf(fp, "%d\n", webpage_getDepth(page));
        fprintf(fp, "%s\n", webpage_getHTML(page));
        fclose(fp);
    }
    free(filename);
}
bool pagedir_validate(const char *dir)
{

    // return error message  for NULL directory

    // get path name here and store on stack
    char path[strlen(dir) + 10];              // create string with enough space for directory and /.crawler
    sprintf(path, "%s/%s", dir, "/.crawler"); // put the path in memory
    

    if (access(path, F_OK) == 0)
    {
        return true;
    }
    return false;
}

// Function to load a webpage from a given directory and document ID
webpage_t* pagedir_load(const char* pageDirectory, const int docID) {
    // Allocate memory for the pathname
    char* pathname = (char*)malloc(strlen(pageDirectory) + sizeof(docID) + 10);

    // Create the pathname by combining the page directory and document ID
    sprintf(pathname, "%s/%d", pageDirectory, docID);

    // Open the file for reading
    FILE* fp = fopen(pathname, "r");

    // Free the memory allocated for the pathname (cleanup)
    if (pathname != NULL) {
        mem_free(pathname);
    }

    // Check if the file was opened successfully
    if (fp != NULL) {
        // Read the URL, depth, and HTML content from the file
        char* URL = file_readLine(fp);
        char* depth = file_readLine(fp);
        char* html = file_readFile(fp);

        // Create a new webpage using the read data
        webpage_t* webpage = webpage_new(URL, atoi(depth), html);

        // Close the file
        fclose(fp);

        // Free the memory allocated for the depth (cleanup)
        free(depth);

      

        // Return the loaded webpage
        return webpage;
    }

    // Return NULL if the file couldn't be opened or other errors occurred
    return NULL;
}
