# Set Module
# Brenda Waiya 
# 

## Overview

The **Set module** provides a data structure for maintaining a collection of unique key-item pairs. It allows for inserting, searching, iterating, and deleting key-item pairs.

## Data Structures

### `setnode_t`

- Represents a node within the set.
- Contains:
  - `char* key`: The key associated with the item.
  - `void* item`: The data associated with the key.
  - `setnode_t* next`: A pointer to the next node in the set.

### `set_t`

- Represents the set itself.
- Contains:
  - `setnode_t* head`: A pointer to the head of the set.

## Functions

The module exports the following functions:

### `set_t* set_new(void);`

Creates a new, empty set and returns a pointer to it.

### `bool set_insert(set_t* set, const char* key, void* item);`

Inserts an item identified by a key (string) into the set.

- Returns `false` if the key already exists, or any parameter is `NULL`, or an error occurs.
- Returns `true` if the item is successfully inserted.

### `void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item));`

Iterates over the set, calling a provided function on each item.

### `void set_delete(set_t* set, void (*itemdelete)(void* item));`

Deletes the entire set, calling a delete function on each item.

### `void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item));`

Prints the entire set to a specified file, using a provided function to print each item.

### `void* set_find(set_t* set, const char* key);`

Finds and returns the item associated with the given key.

## Implementation Details

- The set is implemented as a singly linked list of `setnode_t` nodes.
- Each node stores a key and an associated item.
- Key strings are copied for use by the set, and the module is responsible for allocating and deallocating memory for them.
- The set maintains uniqueness based on keys.

## Compilation and Testing

To compile the module, simply run `make set.o`.

For testing, the module includes a test program, `settest.c`, which tests the various functions of the Set module. To run the tests, use the command `make test`.

## Files

- `Makefile`: Compilation procedure
- `set.h`: The module's interface
- `set.c`: The module's implementation
- `settest.c`: Unit test driver

