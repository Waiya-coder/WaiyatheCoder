// Name: Brenda Waiya
// Class: CS50 Fall 2023


#ifndef QUERIER_H
#define QUERIER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../common/index.h"
#include "../libcs50/counters.h"
#include "../common/pagedir.h"
#include "../libcs50/file.h"
#include "../common/word.h"

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

#endif 