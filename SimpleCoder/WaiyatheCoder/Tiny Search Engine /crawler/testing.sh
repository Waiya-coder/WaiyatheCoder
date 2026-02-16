#!bin/bash
#Name: Brenda Waiya
#Date: 10/15/2023
# Purpose: testing file for crawler. 
# Usage: bash -v testing.sh


#Remove the directory that might already exist and create new directories
rm -rf ../data
mkdir -p ../data/letters
mkdir -p ../data/toscrape
mkdir -p ../data/wikipedia

# No arguments
# ./crawler

# Less than 4 arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters

# Invalid URL
 ./crawler http://cs50xfe.cs.dartmouth.com/tse/letters/index.html ../data/letters 10

# Invalid directory
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../nonexistent/letters 10

# Testing with letters as seedURL at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 0
#./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-3 10

# Testing with letters as seedURL at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 1

# #Testing with letters as seedURL at depth 2
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2

# # Testing with letters as seedURL at depth 10
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 10

# Testing at different seed URL
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/D.html ../data/letters 2

# Testing with toscrape as seedURL at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape ../data/toscrape 0

# Testing with toscrape as seedURL at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape ../data/toscrape 1

# # Testing with toscrape as seedURL at depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape ../data/toscrape 2

# # Testing with toscrape as seedURL at depth 3
 ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape ../data/toscrape 10

# # Testing with wikipedia as seedURL at depth 0
 ./crawler https://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia 0
x
# # Testing with wikipedia as seedURL at depth 1
 ./crawler https://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia 1

# # Testing with wikipedia as seedURL at depth 2
 ./crawler https://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia 2

# Run with valgrind over toscrape at depth 1
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape ../data/toscrape 1



exit 0