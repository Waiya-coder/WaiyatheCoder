# Hashtable Module
# Brenda Waiya
# 10/06/2023
## Overview

The **Hashtable module** provides a data structure for storing key-value pairs, where keys are unique and are used to retrieve associated values efficiently. It implements a hashtable using a set of slots, and each slot contains a set of key-item pairs.

## Data Structures

### `hashtable_t`

- Represents the hashtable itself.
- Contains:
  - `int num_slots`: The number of slots in the hashtable.
  - `set_t** table`: An array of sets, where each set stores key-item pairs.

## Functions

The module exports the following functions:

### `hashtable_t* hashtable_new(const int num_slots);`

Creates a new (empty) hashtable with the specified number of slots.

- Returns a pointer to the new hashtable or `NULL` if an error occurs.
- The caller is responsible for later calling `hashtable_delete` to free the memory used by the hashtable.

### `bool hashtable_insert(hashtable_t* ht, const char* key, void* item);`

Inserts an item identified by a key (string) into the given hashtable.

- Returns `false` if the key already exists in the hashtable, any parameter is `NULL`, or an error occurs.
- Returns `true` if the new item was successfully inserted.

### `void* hashtable_find(hashtable_t* ht, const char* key);`

Returns the item associated with the given key from the hashtable.

- Returns a pointer to the item if found; otherwise, returns `NULL`.
- The hashtable is unchanged by this operation.

### `void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item));`

Prints the entire hashtable to a specified file, using a provided function to print each key-item pair.

### `void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item));`

Iterates over all items in the hashtable in an undefined order, calling a provided function on each key-item pair.

### `void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item));`

Deletes the entire hashtable, calling a delete function on each item.

## Implementation Details

- The hashtable is implemented as an array of sets.
- Keys are hashed to determine which slot they should be placed in.
- The module ensures that each key is unique within the hashtable.
- Memory for key strings is allocated and deallocated by the module itself.

## Compilation and Testing

To compile the module, simply run `make hashtable.o`.

For testing, the module includes a test program, `hashtabletest.c`, which tests the various functions of the Hashtable module. To run the tests, use the command `make test`.

## Files

- `Makefile`: Compilation procedure
- `hashtable.h`: The module's interface
- `hashtable.c`: The module's implementation
- `hashtabletest.c`: Unit test driver

## Example Usage

Please refer to the provided `hashtabletest.c` file for an example of how to use the Hashtable module to create, insert, find, iterate, and delete key-item pairs within the hashtable.

