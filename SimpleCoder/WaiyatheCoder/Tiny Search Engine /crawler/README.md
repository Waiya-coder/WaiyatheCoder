# Brenda Waiya
# Web Crawler
## Wikipedia 



## Introduction

This is a simple web crawler program written in C. It is designed to crawl web pages starting from a seed URL and save them to a specified directory up to a maximum depth.

## Usage

To use the web crawler, run the program from the command line with the following arguments:

```shell
./crawler seedURL pageDirectory maxDepth
- `seedURL`: The starting URL for crawling.
- `pageDirectory`: The directory where crawled web pages will be saved.
- `maxDepth`: The maximum depth to crawl (a non-negative integer).
```
## Each function and their use 
## parseArgs Function
This function parses the command-line arguments passed to the program. 


## crawl Function
The crawl function is responsible for the core web crawling logic. It performs the following steps:

Initializes data structures like bags and hashtables to manage URLs and visited pages.
Creates a seed webpage with a depth of 0 and adds it to the bag of URLs to crawl.
Iteratively extracts webpages from the bag, fetches their content, and saves them to the specified directory.
Scans the HTML content of the webpages for new URLs to crawl.
Continues crawling until it reaches the specified maximum depth.
Logs information about each step of the crawling process, including fetching and scanning.
## pageScan Function
The pageScan function is responsible for scanning the HTML content of a webpage for links (URLs) and adding them to the bag of URLs to crawl if they meet the specified criteria


## logword Function: i got it from crawler-loggin.md
These was not one of the codes provided in the instructions but in the absence, I wasn't able to see log messages about each step of the process. its absence did not affect the core functionality of the code. Because it still parsed arguments, crawled webpages, and scan for links as expected, but  The logword function enhanced my code's usability by providing a way to monitor and understand what the program is doing during execution.

## wikipedia testing.sh
According to the output it is not an internal link and I can't really figure out how to fix that

## 





