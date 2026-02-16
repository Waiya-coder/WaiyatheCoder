# Counters Module
# Brenda Waiya CS50 FALL 2023
# 10/06/2023

## Overview

The **counters module** provides a data structure for counting occurrences of keys. It is used to keep track of the frequency of keys in a set.

## Functions

The module exports the following functions:

### `counters_t* counters_new(void);`

Creates a new empty counterset and returns a pointer to it.

### `int counters_add(counters_t* ctrs, const int key);`

Increments the count associated with the given `key` in the counterset `ctrs`. If the `key` does not exist in the counterset, it is added with a count of 1.

### `int counters_get(counters_t* ctrs, const int key);`

Returns the current count associated with the given `key` in the counterset `ctrs`.

### `bool counters_set(counters_t* ctrs, const int key, const int count);`

Sets the count associated with the given `key` in the counterset `ctrs` to the specified `count` value. If the `key` does not exist in the counterset, it is added with the specified `count`.

### `void counters_print(counters_t* ctrs, FILE* fp);`

Prints the contents of the counterset `ctrs` to the specified file `fp` in the format: `{[key1:count1],[key2:count2],...}`.

### `void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count));`

Iterates over all counters in the counterset `ctrs`, calling the provided `itemfunc` function for each item. The `itemfunc` function is called with arguments `arg`, `key`, and `count`.

### `void counters_delete(counters_t* ctrs);`

Deletes the entire counterset `ctrs`, freeing all memory associated with it.

## Implementation Details

The counters module implements the counterset as a singly linked list. Each node in the list, referred to as `counternode`, contains a `key` and a `count`.

## Compilation and Testing

To compile the module, simply run `make test`.

For testing, the module includes a test program, `countertest.c`, which reads input data and tests the various functions of the counters module. To run the tests, use the command `make test`. You can also enable memory testing by editing the Makefile to turn on the `MEMTEST` flag and then running `make test`.

## Files

- `Makefile`: Compilation procedure
- `counters.h`: The module's interface
- `counters.c`: The module's implementation
- `countertest.c`: Unit test driver
- `test.names`: Test data
- `testing.out`: Result of `make test &> testing.out`
