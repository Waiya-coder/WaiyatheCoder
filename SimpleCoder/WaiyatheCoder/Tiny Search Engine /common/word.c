// Name: Brenda Waiya
// Class: CS50 Fall 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

/**************** functions ****************/

/**************** normalize_word ****************/
char* normalize_word(char* word)
{
   
    for(int i = 0; i <= (strlen(word)); i++) {
        word[i] = tolower(word[i]);
    }

    return word;
}
