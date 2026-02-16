# CS50 TSE Querier Implementation Spec
# Name: Brenda Waiya
# Class: CS50 Fall 2023
In this document, we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. We focus on the core subset of topics:

## Data Structures

The implementation of the CS50 TSE Querier involves the use of various data structures to manage queries, indices, counters, and search results. These data structures include:

- `counters_t`: A data structure to store document sets and their counts.
- `index_t`: A data structure representing the search index.
- `DocumentScore_t`: A structure to hold document identifiers and their corresponding scores.
- `char *[]`: An array of pointers to store individual words from user queries.

## Control Flow: Pseudo Code for Overall Flow, and for Each of the Functions

### Overall Flow

The overall flow of the CS50 TSE Querier implementation can be summarized in the following pseudocode:
## Functions

### `validateCommandLineArguments(argc, argv)`

- Validate the command-line arguments to ensure they are correct.
- Print an error message and exit if the arguments are invalid.

### `readUserQueryInput()`

- Read a line of user input, representing a query, from the command line.

### `splitQueryIntoWords(queryInput)`

- Split the user query input into individual words.
- Return an array of pointers to the words.

### `isQueryValid(arrayOfWords)`

- Validate the structure of the user query.
- Ensure that there are no adjacent "ANDs" or "ORs" and that the query follows a valid structure.
- Return true if the query is valid, otherwise return false.

### `querryProcessor(index, arrayOfWords)`

- Process the user query and return matching documents from the search index.
- Perform operations based on "AND" and "OR" sequences to find relevant documents.
- Return a counters data structure representing the search results.

### `print_Counter_Result(countersone, pageDirectory)`

- Print the search results in the specified format, including relevance scores and document details.

### Other Helper Functions

- There are various helper functions for union, intersection, and other operations on counters and data structures.

### Detailed Function Prototypes and Their Parameters

The detailed function prototypes, including parameter descriptions, can be found in the Design Specification document.

## Error Handling and Recovery

The implementation should handle errors gracefully. This includes:

- Validating command-line arguments and providing clear error messages for incorrect arguments.
- Ensuring that memory allocation functions (malloc, calloc, etc.) are checked for failure and handled appropriately.
- Implementing error checks for file operations, such as loading the search index file.
- Managing errors related to invalid queries, including structure and content errors.

## Testing Plan

Testing is a critical aspect of the implementation to ensure correctness and reliability. The following testing plan outlines various types of tests:


- **Edge Cases Testing**: Test the system with edge cases, including empty queries, queries with invalid structures, and large datasets.



- **Performance Testing**: Evaluate the search engine's performance with a variety of input queries and datasets to ensure efficiency.

The testing plan should cover a wide range of scenarios to validate the functionality and robustness of the CS50 TSE Querier implementation.

## Data Structures

Please refer to the "Data Structures" section above for details on the data structures used in the implementation.
