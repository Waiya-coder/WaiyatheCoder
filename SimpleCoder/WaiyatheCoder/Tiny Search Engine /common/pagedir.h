// Name: Brenda Waiya
// Class: CS50 Fall 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"





bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* dir);
webpage_t* pagedir_load(const char *pageDirectory, const int docID);



