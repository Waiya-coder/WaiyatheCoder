# Nuggets
## Bowling, Fall 2023

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Materials provided

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.

## NOTES

The map we created is `bowlingmap.txt`

## Implementation

We followed the IMPLEMENTATION.md spec.

## Errors

There are memory leaks coming from swapping players.

There are memory errors coming from spectator -- they relate to the socket and sending messages.

There are 2 random seeds, including 30948, that leaves a piece of gold uncollected. We assume it is within srand().