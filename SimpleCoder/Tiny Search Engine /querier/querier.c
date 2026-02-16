// Name: Brenda Waiya
// Class: CS50 Fall 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "querier.h"
#include "../libcs50/mem.h"

/**************** local types ****************/

// A structure to facilitate intersection operations between counters
typedef struct doubleCounters
{
    counters_t *first;  // Pointer to the first counters_t structure
    counters_t *second; // Pointer to the second counters_t structure
} doubleCounters_t;

// Helper struct to hold document identifiers and their corresponding scores
typedef struct DocumentScore
{
    int documentID; // Document identifier
    int score;      // Corresponding score for the document
} DocumentScore_t;

/**************** local functions ****************/

// ProcessQuery: Main query processing loop
static void ProcessQuery(char *pageDirectory, const char *indexFilename);

// findMin: Helper function to find the minimum of two integers
static inline int findMin(const int num1, const int num2);

// numberItems: Helper function to increment a counter that tracks the number of items with a positive score
static void numberItems(void *arg, const int key, const int count);

// findMax: Helper function responsible for updating the highest score and corresponding document ID
static void findMax(void *arg, const int documentID, const int score);

// printHelperFunc: Helper function to print query results for a specific document
static void printHelperFunc(const int score, const int documentID, char *pageDirectory);

/**************** global functions ****************/

// querryProcessor: Process the query and return counters for search results
counters_t *querryProcessor(index_t *index, char **arrayOfWords, int wordNumbers);

// split_Words: Split the query input into individual words and count them
int split_Words(char **arrayOfWords, char *queryInput);

// isQueryValid: Validate the structure of the query (e.g., no adjacent "ANDs" or "ORs")
bool isQueryValid(char **arrayOfWords, int wordNumbers);

// mergeCounter: Merge 'AND' and 'OR' sequences
void mergeCounter(counters_t **andIntersectSeq, counters_t **orUnionSeq);

// countersUnionFunc: Union operation for counters
void countersUnionFunc(counters_t *one, counters_t *two);

// countersUnionFuncHelper: Helper function for union operation
void countersUnionFuncHelper(void *arg, const int key, const int count);

// counterIntersectFunc: Intersection operation for counters
void counterIntersectFunc(counters_t *one, counters_t *two);

// counterIntersectFuncHelper: Helper function for intersection operation
void counterIntersectFuncHelper(void *arg, const int key, const int count);

// print_Counter_Result: Print query results
void print_Counter_Result(counters_t *countersone, char *pageDirectory);

// isAnd: Check if a word is "and"
bool isAnd(char *word);

// isOr: Check if a word is "or"
bool isOr(char *word);

// is_and_or: Check if a word is "AND" or "OR"
bool is_and_or(char *word);

/**************** main() ****************/
int main(const int argc, char *argv[])
{
    // Validate the number of command-line arguments
    if (argc != 3)
    {
        fprintf(stderr, "Error: Usage %s: [pageDirectory] [indexFileName]\n", argv[0]);
        exit(1);
    }
    // initialize the arguments
    //  Extract and validate the page directory argument
    char *pageDirectory = argv[1];
    if (pageDirectory == NULL)
    {
        fprintf(stderr, "PageDirectory is Null\n");
        exit(2);
    }

    // check if its a valid directory
    if (!pagedir_validate(pageDirectory))
    {
        fprintf(stderr, "Error:Directory given is not a crawler directory\n");
        exit(3);
    }
    // Extract index filename and validate its writability
    char *indexFilename = argv[2];
    // Open the file for reading
    FILE *fp = fopen(indexFilename, "r");
    // Check for errors during file opening
    if (fp == NULL)
    {
        fprintf(stderr, "Indexfile given is not readable");
        exit(4);
    }
    fclose(fp);

    ProcessQuery(pageDirectory, indexFilename);

    return 0;
}

/**************** ProcessQuery() ****************/
/* Main query processing loop */
static void ProcessQuery(char *pageDirectory, const char *indexFileName)
{
    char *queryInput; // Variable to store the query input from standard input

    printf("Query: "); // Print the initial query prompt to the user

     // Main loop: Continue reading lines from standard input until a NULL is encountered
    while ((queryInput = file_readLine(stdin)) != NULL)
    {
        // Load the search index from a file
       index_t *index = index_new(500); // Initialize your index (if not already done)

  if (index == NULL) {
    // Handle index initialization failure
    fprintf(stderr, "Index is Null\n");
    exit(1);
}

index_load((char*)indexFileName, index); // Pass both arguments to index_load


        // Allocate memory for an array to hold individual words in the query
        char **arrayOfWords = malloc(strlen(queryInput) * sizeof(char *));

        // Split the query input into individual words and count them
        int wordNumbers = split_Words(arrayOfWords, queryInput);

        // Validate the structure of the query (e.g., no adjacent "ANDs" or "ORs")
        if (isQueryValid(arrayOfWords, wordNumbers))
        {
             // Process the query and store the result in counters
            if (wordNumbers > 0)
            {
                // get the counters [documentID, count] and print the MatchesMade
                counters_t *countersone = querryProcessor(index, arrayOfWords, wordNumbers);

                // Print the search results
                print_Counter_Result(countersone, pageDirectory);
            }
        }
        
        
        // Free the dynamically allocated memory
        free(queryInput);

        // for(int i = 0; i < wordNumbers; i++) {
        //    e(arrayOfWords[i]);
        // }
        free(arrayOfWords);

        // Delete the loaded index to free up memory
        index_delete(index);

        // Print a new query prompt for the next iteration
        printf("\n\nQuery: ");
    }
    printf("\n"); // Print a final newline when exiting the loop
}

/**************** *querryProcessor() ****************/
/* see querier.h for description */
counters_t *querryProcessor(index_t *index, char **arrayOfWords, int wordNumbers)
{
    // Initialize counters for 'AND' and 'OR' sequences
    counters_t *andIntersectSeq = NULL; //and
    counters_t *orUnionSeq = NULL;//or

    //initialize the bool reset flag
    bool reset = false;

    //// Loop through each word in the array
    for (int i = 0; i < wordNumbers; i++)
    {
        // handle the or case
        if (strcmp(arrayOfWords[i], "or") == 0)
        {
            //Merge existing 'AND' sequence into 'OR' sequence
            mergeCounter(&andIntersectSeq, &orUnionSeq);

            // Reset the flag for the next sequence
            reset = false;
            continue; // skip to the next iteration
        }
        // Skip word if reset flag is true until or
        if (reset)
        {
            continue;
        }
        // Skip 'and' keyword
        if (strcmp(arrayOfWords[i], "and") == 0)
        {
            continue;
        }
        else
        {
            // Find the matching pages for the current word
            counters_t *pageMatch = index_find(index, arrayOfWords[i]);
           
            if (pageMatch == NULL) // Handle no match case
            {
                reset = true; //reset the flag to true

                 // Clear the existing 'AND' sequence
                if (andIntersectSeq != NULL)
                {
                    counters_delete(andIntersectSeq); //delete
                    andIntersectSeq = NULL; //set it back to null
                }
            }
            else
            {
                // Initialize 'AND' sequence if needed, nothing was previously here
                if (andIntersectSeq == NULL)
                {
                    //create a new and sequence
                    andIntersectSeq = counters_new();
                    countersUnionFunc(andIntersectSeq, pageMatch);
                }
                else
                {
                    // Finally Intersect with the existing 'AND' sequence
                 counterIntersectFunc(andIntersectSeq, pageMatch);
                }
            }
        }
    }
    // Final merge of remaining 'AND' sequence into 'OR' sequence
    mergeCounter(&andIntersectSeq, &orUnionSeq);
    return orUnionSeq;
}

/**************** split_Words() ****************/
/* see querier.h for description */
int split_Words(char **arrayOfWords, char *queryInput)
{
    int wordNumbers = 0;   // Initialize the word count to 0        
    int length = strlen(queryInput);  // Get the length of the query input string

    // Loop through each character in the query input
    for (int i = 0; i <= length; i++)
    {
        // Skip white spaces
        if (isspace(queryInput[i]))
        {
            i++;
        }
        // Check if the character is alphabetical
        if (isalpha(queryInput[i]))
        {
            char *word = &queryInput[i];// Store the beginning of the word

            // Loop until a space or null terminator is found
            while (!isspace(queryInput[i]))
            {
                // Break if we reach the end of the string
                if (queryInput[i] == '\0')
                {
                    break;
                }
                i++;
            }
            
            // Null-terminate the word
            queryInput[i] = '\0';

            // normalized the word 
            char *normalized = normalize_word(word);

            //add normaliozed word to array
            arrayOfWords[wordNumbers] = normalized;
            printf("%s ", normalized); // print  word
            //e(normalized);

            wordNumbers++; // increment 
        }
        else{
            fprintf(stderr, "Invalid Character. Character should either be an Alphabet or a space\n");
            return -1;
        }
    }
    return wordNumbers; // return word count
}

/**************** bool isAnd() ****************/
/* see querier.h for description */
bool isAnd(char *word)
{
    // Check for NULL input
    if (word == NULL)
    {
        return false;
    }

    // Compare word to "and"
    if ((strcmp(word, "and")) == 0)
    {
        return true;
    }

    // Default case: word is not "and"
    return false;
}


/****************bool isOr() ****************/
/* see querier.h for description */
bool isOr(char *word)
{
    // Check for NULL word
    if (word == NULL)
    {
        return false;
    }

     // Compare word to "or"
    if ((strcmp(word, "or")) == 0)
    {
        return true;
    }

    // Default case: word is not "or"
    return false;
}

// function that checks if the word is and or or
/**************** is_and_or() ****************/
/* see querier.h for description */
bool is_and_or(char *word)
{
    // Step 1: Null Check
    // Check if the input word is NULL. If it is, return false immediately.
    if (word == NULL)
    {
        return false; // A NULL word cannot be 'AND' or 'OR', so return false.
    }

    // Step 2: Check for 'AND' or 'OR'
    // Utilize helper functions isAnd() and isOr() to determine if the word is 'AND' or 'OR'.
    if (isAnd(word) || isOr(word))
    {
        return true; // The word is either 'AND' or 'OR', so return true.
    }

    // Step 3: Default Return
    // If the function has not returned by this point, the word is neither 'AND' nor 'OR'.
    return false;
}

/**************** isQueryValid() ****************/
/* see querier.h for description */
bool isQueryValid(char **arrayOfWords, int wordNumbers)
{
    // Iterate through each element in the array.
    for (int i = 0; i < wordNumbers; i++)
    {
         // Check if 'AND' or 'OR' is at the beginning of the array, which is invalid.
        if ((i == 0 && isOr(arrayOfWords[i])) || (i == 0 && isAnd(arrayOfWords[i])))
        {
            // Print an error message to stderr and return false to indicate an invalid structure.
            fprintf(stderr, "Error:  %s Should not be at the beginning of the string array \n", arrayOfWords[i]);
            return false; // Invalid query structure, exit early.
        }

        // General case: Check for adjacent 'AND' or 'OR'.
        // Make sure we are not exceeding array bounds with i + 1.
        if (i + 1 < (wordNumbers - 1))
        {
            // Both adjacent elements cannot be 'AND' or 'OR'.
            if (is_and_or(arrayOfWords[i]) && is_and_or(arrayOfWords[i + 1]))
            {
                // Print an error message to stdout and return false.
                printf("Error: Two consecutive words cannot both be operators ('AND' or 'OR').\n");
                return false; // Invalid query structure, exit early.
            }
        }

        // Special case: Check the last element.
        // 'AND' or 'OR' cannot be at the end of the array.
        if (i == (wordNumbers - 1))
        {
            if ((isOr(arrayOfWords[i])) || (isAnd(arrayOfWords[i])))
            {
                // Print an error message to stdout and return false.
                printf("Error:  %s should not be at the end of the string array \n", arrayOfWords[wordNumbers - 1]);
                return false; // Invalid query structure, exit early.
            }
        }
    }
    // If the function has not returned false by this point, the array is valid.
    return true; // this is now a valid query
}

/**************** mergeCounter() ****************/
/* see querier.h for description */
void mergeCounter(counters_t **andIntersectSeq, counters_t **orUnionSeq)
{
    // Check if andIntersectSeq is not NULL before proceeding.
    if (*andIntersectSeq != NULL)
    {
        // Initialize orUnionSeq if it is NULL.
        if (*orUnionSeq == NULL)
        {
            *orUnionSeq = counters_new();
            // Create a new counter.
        }

        // Merge the two counters by performing a union operation.
        countersUnionFunc(*orUnionSeq, *andIntersectSeq);

        // Delete the andIntersectSeq counter
        counters_delete(*andIntersectSeq);
        *andIntersectSeq = NULL; // // Nullify the pointer to indicate that it no longer points to a valid counter.
    }
}

/**************** countersUnionFunc) ****************/
/* see querier.h for description */
void countersUnionFunc(counters_t *one, counters_t *two)
{
    // Assert that the memory for the first counter is valid.
   mem_assert(stderr, "Memory of the first counter is invalid\n");

    // Assert that the memory for the second counter is valid.
   mem_assert(stderr, "Memory of the second counter is invalid\n");

    /* Iterate through the first counter and update its values based on 
    the union with the second counter. */
    counters_iterate(two, one, countersUnionFuncHelper);
}


/**************** countersUnionFuncHelper() ****************/
/* see querier.h for description */
void countersUnionFuncHelper(void *arg, const int key, const int occurenceCount)
{
    /* Update the count for the word (key) in the counter by summing its current count 
    and the count from another counter.*/
    counters_set(arg, key, occurenceCount + counters_get(arg, key));
}

/**************** counterIntersectFunc() ****************/
/* see querier.h for description */
void counterIntersectFunc(counters_t *one, counters_t *two)
{
    // Assert that the memory for the first counter is valid.
   mem_assert(stderr, "Memory of the first counter is invalid");

    // Assert that the memory for the first counter is valid.
   mem_assert(stderr, "Memory of the second counter is invalid");

    // create two counters
    struct doubleCounters args = {one, two};

    /* Iterate through the first counter and update its values based on 
    the intersection with the second counter.*/
    counters_iterate(one, &args, counterIntersectFuncHelper);
}

/************** findMin() ***************/
/**  Designed to return the smaller of two integers.*/
static inline int findMin(const int num1, const int num2)
{
    // Use the ternary conditional operator to compare num1 and num2,
    // returning the smaller of the two.
    return (num1 < num2 ? num1 : num2);
}

/**************** counterIntersectFuncHelper() ****************/
/* see querier.h for description */
/* The counterIntersectFuncHelper function finds the intersection of two counters sets and updates one of them accordingly. */
void counterIntersectFuncHelper(void *arg, const int key, const int count)
{
    // Type casting the generic pointer to a specific structure type
    struct doubleCounters *ctr = arg;

    // Set the value in the first counter to be the minimum between the occurrenceCount and the 
    //corresponding value in the second counter
    counters_set(ctr->first, key, findMin(count, counters_get(ctr->second, key)));
}


/************** print_Counter_Results() ***************/
/** The print_Counter_Results function prints and ranks documents based on their scores */
void print_Counter_Result(counters_t *countersone, char *pageDirectory)
{
    // Validate that the counters set is not null
    if (countersone != NULL) 
    {
        int MatchesMade = 0; // Initialize a counter for the number of documents

        // Count the number of documents with a positive score
        counters_iterate(countersone, &MatchesMade, numberItems);

        // Print the total number of matching documents
        printf("\nNumber of Matches %d Ranked documents: \n", MatchesMade);

        // Dynamically allocate memory for storing the largest score and corresponding document ID
        DocumentScore_t *highest = malloc(sizeof(DocumentScore_t));
        highest->documentID = 0; // Initialize the document ID to 0
        highest->score = 0; // Initialize the score to 0

        // Loop through each document to find and print the largest score
        for (int i = 0; i < MatchesMade; i++)
        {
            //  Find the document with the largest score
            counters_iterate(countersone, highest, findMax);

            // Print the details of the document with the largest score
         printHelperFunc(highest->score, highest->documentID, pageDirectory);

            // Reset the score of the document just printed to zero
            counters_set(countersone, highest->documentID, 0); // Reset the largest score to 0
            highest->score = 0;
        }

        free(highest); // Free dynamically allocated memory
        counters_delete(countersone); // Delete the counters set
    }
    else
    {
        // If the counters set is null, print that no documents match
        printf("\nNo documents match.\n");
    
    }
}

/************** numberItems() ***************/
/**   increment a counter that tracks the number of items with a positive score. */
static void numberItems(void *arg, const int key, const int count)
{
     // Cast the void pointer arg to a pointer to an integer
    int *MatchesMade = arg;

    // Check if the pointer is not NULL and the count (score) is greater than 0
    if (MatchesMade != NULL && count > 0)
    {
        (*MatchesMade)++; // Increment the counter pointed to by allItems
    }
}

/************** findMaxScore() ***************/
/** Responsible for updating the highest score and corresponding document ID.*/
static void findMax(void *arg, const int documentID, const int score)
{
    // Cast the void pointer arg to a pointer of type DocumentScore_t
    DocumentScore_t *highest = arg;

    // Check if the current score (count) is greater than or equal to the current highest score stored in highest->score
    if (highest->score <= score)
    {
        // Update the document ID in the highest structure to the current document ID (key)
        highest->documentID = documentID;

        // Update the highest score in the highest structure to the current score (count)
        highest->score = score;
    }
}

/************** printHelperFunc ***************/
/** prints the score and document information associated with a particular document ID.  */
static void printHelperFunc(const int score, const int documentID, char *pageDirectory)
{
    
    FILE *fp; // Declare a file pointer

    // Dynamically allocate memory for the file name
    char *filename = malloc(strlen(pageDirectory) + strlen("/") + sizeof(int) + 1);

    // Format the string to form the full path of the file
    sprintf(filename, "%s/%d", pageDirectory, documentID);

    // Open the file for reading
    fp = fopen(filename, "r");
    if (fp != NULL) // Check if the file was opened successfully
    {
        // Read the first line from the file, which is presumably the URL
        char *URL = file_readLine(fp); 

       // Print the score, document ID, and URL to the standard output
        printf("score:\t%d doc\t%d: %s\n", score, documentID, URL);
        free(URL); //free the url
        fclose(fp); //clese the file pointer
    }
   free(filename); //free the file nme
}