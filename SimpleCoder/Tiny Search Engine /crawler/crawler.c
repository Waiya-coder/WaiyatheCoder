// Name: Brenda Waiya
// Class: CS50 Fall 2023
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../common/pagedir.h"

static void parseArgs(const int argc, char *argv[],
                      char **seedURL, char **pageDirectory, int *maxDepth);
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth);
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);
static void logr(const char *word, const int depth, const char *url);

int main(const int argc, char *argv[])
{
    printf("Test run");
    char *seedURL = argv[1];       // seedURL
    char *pageDirectory = argv[2]; // pageDirectory
    int maxDepth;                  // maxDepth

    // Parse Arguments
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    // Crawl from seedURL to maxDepth and save pages in pageDirectory
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}

/**************** parseArgs() ****************/
/* see crawler.h for description */
static void parseArgs(const int argc, char *argv[],
                      char **seedURL, char **pageDirectory, int *maxDepth)
{

    char *program = argv[0]; // the object name, in this case crawler 

    // Check number of arguments
    if (argc != 4)
    {
        fprintf(stderr, "Wrong number of arguments.\n");
        fprintf(stderr, "Usage: %s [seedURL] [pageDirectory] [maxDepth]\n", program);
        exit(1);
    }

    // Check URL validity
    char *url = normalizeURL(*seedURL);
    if (url == NULL)
    {
        fprintf(stderr, "%s it cannot be normalized.\n", url);
        mem_free(url);
        exit(2);
    }
    // Validate if the URL is internal
    if (!isInternalURL(url))
    {
        fprintf(stderr, "%s is not an internal URL.\n", url);
        mem_free(url);
        exit(3);
    }
    // Initialize the seed URL
    mem_free(url);

    // Convert maxDepth into integer
    //
    char unParsedCharater;
    if (sscanf(argv[3], "%d%c", maxDepth, &unParsedCharater) != 1)
    {
        fprintf(stderr, "%s is not a valid depth.\n", argv[3]);
        exit(4);
    }

    // Check maxDepth is within valid range
    if (*maxDepth < 0 || *maxDepth > 10)
    {
        fprintf(stderr, "%d is out of range\n", *maxDepth);
        exit(5);
    }

    // Call pagedir_init()
        if (!pagedir_init(*pageDirectory))
    {
      fprintf(stderr, "Unable to construct the pathname for the .crawler file in the directory %s\n", *pageDirectory);
        exit(6);
    }
}

/**************** crawl() ****************/
/* see crawler.h for description */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth)
{
//create a copy of the seedURL so that you don't mess with the original
    char *urlCopy = mem_malloc(sizeof(char) * strlen(seedURL) + 1);
    strcpy(urlCopy, seedURL);

   
    char *item = "";   // Null String
     const int slots = 200;       // number of slots for the hashtable 
    hashtable_t *seen_urls = hashtable_new(slots); // intialize Hashtable of seen URLs                                 
    bag_t *urlBags = bag_new();                 // intialize Bag of URLs
    

    // Check if the bag and hashtable were correctly initialized.
    urlBags = mem_assert(urlBags, "Failed to create bag, check bag_new()\n");
    seen_urls = mem_assert(seen_urls, "Failed to create hashtable, check, hashtable_new()\n");

    // Add the URL to the hashtable
    hashtable_insert(seen_urls, urlCopy, item);

    // Create a webpage representing a seed URL at depth 0 
    webpage_t *webpage = webpage_new(urlCopy, 0, NULL);

    // add the said webpage webpage representing the seedURL at depth to the bag of urls 
    bag_insert(urlBags, webpage);

    int docID = 1; // Tracks the id of the pages
    // Get a webpage from the bag
    while ((webpage = bag_extract(urlBags)) != NULL)
    {
        // Fetch the page
        if (webpage_fetch(webpage))
        {
            logr("Fetched", webpage_getDepth(webpage), webpage_getURL(webpage));
            // Save the webpage to pageDirectory
             pagedir_save(webpage, pageDirectory, docID);
             
            if (webpage_getDepth(webpage) < maxDepth)
            {
                logr("Scanning", webpage_getDepth(webpage), webpage_getURL(webpage));
                // Pagescan scans the current webpage to add other linked webpages
                pageScan(webpage, urlBags, seen_urls);
            }
        }
        webpage_delete(webpage);
        docID += 1;
       
    }

    // Delete the hashtable
    hashtable_delete(seen_urls, NULL);
    // Delete the bag
    bag_delete(urlBags, webpage_delete);
}

/**************** pageScan() ****************/
/* see crawler.h for description */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen)
{

    char *item = ""; // Null String
    char *url;
    int pos = 0;

    // Get the next URL from the page
    while ((url = webpage_getNextURL(page, &pos)) != NULL) //ensures that we haven't reached the end 
    {

        logr("Found", webpage_getDepth(page), url);

        // Validate if URL is internal
        if (!isInternalURL(url))
        {
            logr("IgnExtrn", webpage_getDepth(page), url); //if it's not internal ignore external so that 
            mem_free(url);                      // free the url
        }
        else
        {
            // Insert the webpage into the hashtable
            if (hashtable_insert(pagesSeen, url, item))
            {
                // Insert the webpage into the bag
                bag_insert(pagesToCrawl, webpage_new(url, webpage_getDepth(page) + 1, NULL));  //Writing bag_insert(pagesToCrawl, webpage)
                // would insert the webpage pointer directly into the bag without creating a new webpage_t structure.
                logr("Added", webpage_getDepth(page), url);
            }
            else
            {
                logr("IgnDupl", webpage_getDepth(page), url); //if you cannot insert, there is another copy so ignore the duplicate using ignDupl from crawler- loggin.md
                mem_free(url);
            }
        }
    }
}



/*
 * By ensuring a consistent format, and choosing a simple/unique word for each type of line,
  we can post-process the output with grep, awk, and so forth, enabling us to run various checks on the output of the crawler. l
 * got this from crawler- loggin.md
 * */

static void logr(const char *word, const int depth, const char *url)
{
    printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}
