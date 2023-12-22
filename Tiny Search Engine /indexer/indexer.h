#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/webpage.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../libcs50/webpage.h"

// Function to build an index from webpages in a directory
index_t* index_build(char* directory, index_t* index);

// Function to index a single webpage
void indexPage(webpage_t* webpage, index_t* index, int ID);

// Function to parse command-line arguments and validate inputs
void parseArgs(char* directory, char* filename);

