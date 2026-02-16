<!-- # AI Agents @ Dartmouth College
## Problem Set 1, Part III: SimpleCoder

In this assignment, you will build **SimpleCoder**, a CLI coding agent that can help you write code, navigate codebases, and complete various software engineering tasks. This agent will use several useful concepts from the modern AI Agent development stack: tool use, Retrieval-Augmented Generation (RAG), context management, and task planning, etc.

By the end of this assignment, you should have a working coding agent that you should use to build a demo project (we suggest a simple Python project — your choice).

## Overview

SimpleCoder is a ReAct-style agent that combines tool use, semantic code search, context management, and task planning.

## Getting Started

You are provided with:
- `simplecoder/main.py` - The CLI entry point (complete, do not modify)
- `pyproject.toml` - Package configuration (complete, do not modify)

You need to implement:
- Tool functions and schemas
- Semantic code RAG for code search
- Context management with compacting
- Task planning and decomposition
- Manage user permissions for file read/write access, etc. (must support a session-level persistence option)
- The main agent logic

### Set API Key

```bash
export GEMINI_API_KEY="your-key-here" # You can also switch to a different provider, in case you prefer
```

### Intended Usage


Your implementation should support the following task inputs:
```bash
# Basic usage
simplecoder "create a hello.py file"

# With RAG
simplecoder --use-rag "what does the Agent class do?"

# With planning
simplecoder --use-planning "create a web server with routes for home and about"

# Interactive mode
simplecoder --interactive

# Options
simplecoder --help
``` -->


# SimpleCoder Implementation
this is the API key incase you need it t run
export GEMINI_API_KEY="AIzaSyDzth-B9OK48ug_4W346uymoAN5GrdxWeE"      

## What is this?

SimpleCoder is basically a CLI coding assistant that can actually read/write files, search your codebase, and plan out tasks. Think of it like Claude Code but way simpler (and written for a CS assignment lol).

## How it works

### agent.py - The Main Loop

This is where the magic happens. I implemented a **ReAct loop** which is basically:
1. **Think**: The LLM figures out what to do next
2. **Act**: Calls a tool (like read_file or write_file)
3. **Observe**: Gets the result back and decides next step

It just keeps looping until the task is done or it hits max iterations. I used LiteLLM so you can swap between different models (Gemini, OpenAI, etc.) without changing code.

### tools.py - Actually Doing Stuff

Five main tools the agent can use:
- **list_files**: Shows what's in a directory (with cool emojis)
- **read_file**: Reads files and shows content
- **write_file**: Creates/overwrites files (asks permission first!)
- **search_files**: Finds files matching a pattern
- **edit_file**: Changes specific parts of a file without rewriting the whole thing

All of these check with the permission system before doing anything scary.

### rag.py - Smart Code Search

Instead of just doing dumb text search, I used **AST parsing** to be smarter about it:
1. Parse Python files with the `ast` module
2. Pull out functions and classes as separate chunks
3. Make embeddings for each chunk
4. When you search, find the most similar chunks

This is way better than just splitting text randomly because it keeps functions/classes intact. Falls back to basic keyword search if embeddings break.

### context.py - Not Running Out of Tokens

Tracks how many tokens we're using and **summarizes old messages** when we get close to the limit. Always keeps the last few messages untouched so the conversation doesn't get weird.

Without this, long sessions would just crash when you hit the context window.

### planner.py - Breaking Down Tasks

For complicated stuff, this asks the LLM to split the task into smaller steps:
1. "Here's the task, break it into 3-5 subtasks"
2. Parse the response into a list
3. Execute them one by one
4. Show progress as you go

Makes it way more likely to succeed on multi-step tasks instead of just winging it.

### permissions.py - Don't Break Everything

Two modes:
- **one-time**: Ask every single time
- **session**: Remember your answer for the session

Basically prevents the agent from yeeting your files without asking. Pretty important when you're testing this on actual code.

## Why I made these choices

**ReAct loop**: It's simple and you can actually see what the agent is thinking. Good for debugging when things go wrong (which they do).

**AST chunking**: Because splitting code in the middle of a function is dumb. Functions are natural units - keep them together.

**Session permissions**: Asking for permission 50 times gets old fast, but you still want safety. This is the middle ground.

**Keep recent messages**: Recent stuff matters more. Old conversation can be summarized to save tokens.

## How to use it

```bash
# Simple task
simplecoder "make me a hello world script"

# With RAG (for searching codebases)
simplecoder --use-rag "where is the login function?"

# With planning (for complex stuff)
simplecoder --use-planning "build a todo app with CLI"

# Interactive mode (keeps going)
simplecoder --interactive
```

## Known issues / TODO

- RAG needs embeddings from LiteLLM (falls back if it fails)
- Planning is pretty basic - doesn't handle complex dependencies
- Context summarization just cuts off old messages (could be smarter)
- Only does AST chunking for Python (would need to add parsers for other languages)

## If I had more time

- Git tools (commit, diff, etc.)
- Support for more languages
- Better context compression (actually summarize with LLM)
- Multiple agents working together
- Remember stuff across sessions

---

**Note**: This was built for CS 89/189 Problem Set 1. It works pretty well for simple tasks but definitely not production-ready. Use at your own risk, I'm not responsible if it deletes your code (that's what the permission system is for tho).

