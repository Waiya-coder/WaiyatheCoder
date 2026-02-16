# CS50 TSE Design Spec
# Name: Brenda Waiya
# Class: CS50 Fall 2023
## Design Spec

In this document, we reference the Requirements Specification and focus on the implementation-independent design decisions. We focus on the core subset of topics:

## User Interface

The search engine will primarily interact with users through a command-line interface. Users will provide input queries to the system, and the system will respond with the search results.

## Inputs and Outputs

**Input**: The search engine takes two command-line arguments:

1. `pageDirectory`: The directory containing the crawled web pages.
2. `indexFilename`: The name of the file containing the search index.

**Output**: The search engine will display search results in the following format:


Query: <user's query>
Matches <number> documents (ranked):
score: <score> doc <documentID>: <URL>
score: <score> doc <documentID>: <URL>
...
## Functional Decomposition into Modules

The search engine's functionality is decomposed into the following modules:

### 1. Query Processor Module

- **Responsibility**: Processes user queries, validates query structure, and performs query operations.
- **Functions**:
  - `ProcessQuery(char *pageDirectory, const char *indexFilename)`: Main query processing loop.
  - `querryProcessor(index_t *index, char **arrayOfWords, int wordNumbers)`: Processes the user query and returns matching documents.
  - `split_Words(char **arrayOfWords, char *queryInput)`: Splits the user query input into individual words.
  - `isQueryValid(char **arrayOfWords, int wordNumbers)`: Validates the structure of the user query.
  - `mergeCounter(counters_t **andIntersectSeq, counters_t **orUnionSeq)`: Merges "AND" and "OR" sequences of document sets.

### 2. Index Loader Module

- **Responsibility**: Loads the search index from a specified file and provides functions to interact with the index data structure.
- **Functions**:
  - `index_load(char *filename, index_t *index)`: Loads the search index from a file.

### 3. Counter Operations Module

- **Responsibility**: Contains functions for performing union and intersection operations on document sets.
- **Functions**:
  - `countersUnionFunc(counters_t *one, counters_t *two)`: Performs a union operation on two sets of counters.
  - `counterIntersectFunc(counters_t *one, counters_t *two)`: Performs an intersection operation on two sets of counters.
  - `print_Counter_Result(counters_t *countersone, char *pageDirectory)`: Prints search results based on relevance scores.

### 4. Result Printer Module

- **Responsibility**: Formats and prints search results.
- **Functions**:
  - `printHelperFunc(const int score, const int documentID, char *pageDirectory)`: Prints details for a specific document.

## Major Data Structures

- `counters_t`: A data structure to store document sets and their counts.
- `index_t`: A data structure representing the search index.
- `DocumentScore_t`: A structure to hold document identifiers and their corresponding scores.
- `char *[]`: An array of pointers to store individual words from user queries.

## Testing Plan


- **Edge Cases**: Test the system with edge cases such as empty queries, and queries with invalid structures

- **Performance Testing**: Evaluate the search engine's performance with a variety of input queries and datasets to ensure efficiency.

