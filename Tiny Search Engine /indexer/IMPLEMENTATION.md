# CS50 TSE: Indexer Implementation Specification
# Brenda Waiya 
# cs50Fall 2023 
This is the implementation specification for the indexer component of the tiny search engine. It contains a basic description of the program, how to run it, and some of my thought processes for implementing these functionalities, including pseudocode for the crawler.

## Purpose

As stated in the README.md in this directory, for a search engine to work, it must consist of three components: a crawler that collects all the pages that can be searched, an indexer that sorts and organizes the data collected by the crawler into a data structure that can be easily manipulated, and a querier that can process user queries and return the desired webpage. This part of the project implements the indexer.

## Usage and Functionality

To run the program, you need to execute the following commands:

```bash
$ make
$ ./indexer CRAWLER_DIR INDEX_PATH

$ make 
$ ./indextest INDEX_FILE NEW_FILE

## Inputs

### Indexer
The indexer requires two arguments to run:

- **CRAWLER_DIR:** A path to a directory created by the crawler. The program validates that this directory was indeed created by the crawler, as it checks for a hidden `.crawler` file. Moreover, it assumes that the files in the directory are labeled from 1 and increment by 1 for each file.

- **INDEX_PATH:** The path to the index file where the indexer will save the contents of its data structure.

### Indextest
The `indextest` program requires two arguments:

- **INDEX_FILE:** A file containing the outputs from a previous index data structure. It needs to be in the correct format of the indexer. If the given file exists, it is assumed that it conforms to this format.

- **NEW_FILE:** The path to where the contents of the newly initialized index file will be written.

## Outputs

If the program runs successfully with a non-zero status, it will output nothing to stdout. Instead, it will save the contents of its newly created index data structure to the given path. This file will be formatted as follows:


word1 id1 count1 id2 count2 id3 count3 ...
word2 id1 count4 id2 count5 id3 count6 ...
word3 id1 count7 id2 count8 id3 count9 ...


## Error Handling and Recovery

- If any errors are associated with the given arguments, the program will print a message to stderr and terminate.

- If there are any errors with memory allocation, the program will print a message to stderr and terminate immediately. This edge case may result in memory leaks.

## Persistent Storage

As stated in the introduction, the program will create a new file whose name is dictated by the user. This file will contain the contents of the index data structure that was created. For later use by the querier, this file can be used to re-initialize an index struct