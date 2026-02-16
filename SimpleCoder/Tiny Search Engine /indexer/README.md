# Indexer README
# Brenda Waiya 
# cs50Fall 2023 
## Introduction


This README provides an overview of the indexer code and highlights a known issue that requires further investigation.

## Issue Description

I am currently experiencing an issue with the indexer code where it does not print anything to the output file as expected. The `index_save` method calls the `itemprint` function, but the code stops executing there because the index appears to be empty. I have diligently reviewed and debugged the code, including functions like `index_build`, `index_page`, and even `index_save`, but I am unable to identify the root cause of this problem.

## Debugging Process
- I have also included several print statements that would help me ensure the method inquestion is running.
- I have conducted a thorough debugging process, reviewing the code step by step.
- I have collaborated with Victor Sanni and Professor Palmer to diagnose and address the issue.
- We have verified the loading of data into the index through functions such as `index_build` and `index_page`.
- Despite our efforts, the issue persists, and we are currently in the process of debugging further to identify the source of the problem.

## Update on debugging
- professor Palmer was able to fix the bug which was :I  allocated a new index in main , then in index_build you allocated and filled a new one! Then when main tried to save the index, it had the first index which was empty.
I have now fixed that. 
##extension
I was given an extension allowance for this because it took me three office hours with Vicot Sanni and even more with Prof Palmer but we still couldn't fix it.
