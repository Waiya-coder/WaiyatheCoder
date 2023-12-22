// Name: Brenda Waiya
// Class: CS50 Fall 2023



#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

/*********************** normalize_word() *************************/
/* Coverts a given word to all-lower case and returns the  word to caller
 * Input: a string , char*, word 
 * Output: lower-case version of the  same word
 * return null toif word is NULL
 */
char* normalize_word(char* word); 

#endif // __WORD_H