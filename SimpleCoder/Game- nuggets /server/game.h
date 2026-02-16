/**
 * Module for header file for game structure (used by Server to store the game set)
 * 
 * Joseph Daher, November 2023
*/
#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "player.h"
#include "../libcs50/set.h"
#include "../support/message.h"

/************ structure prototype **********/
typedef struct game {
    int numRows; // number of rows in the game map
    int numCols; // number of cols in the gme map
    int maxPlayers; // number of maxplayers (26)
    char** map; // Pointer to an array of strings representing the map lines
    set_t* players; // set of player structs, key is address, item is players struct
    int** goldLocations; // array for storing gold locations and values (x,y,amount)
    int numGoldPiles; // number of gold piles (randomly beween min and max num gold piles)
    char playerLetters[26]; // stores player letters "a" to "z"
    int nextLetterIndex; // tracks next letter index to be added
    int startPosition[26][2]; // Array of integers for start positions
    bool inGame; // Boolean to check if the game is active.
    addr_t* spectator; // spectator address
} game_t;

/******** function prototypes ********/
game_t* game_new(int numRows, int numCols, int maxPlayers);
void game_delete(game_t *g);
bool game_fillMapFromFile(game_t *game, char *mapFilename);
void game_initializePlayersStartLocations(game_t* game, int seed);
void game_initializeGold(game_t *game, int seed, int GoldTotal, int GoldMinNumPiles, int GoldMaxNumPiles);
char game_getNextLetter(game_t *game);
char game_addPlayer(game_t *game, const char* playerName, addr_t address);
void game_addSpectator(game_t* game, addr_t* address);
void game_removeSpectator(game_t* game);
char* game_sendScoreTable(game_t* game);

/**
 * function to allocate the memory for an empty game structure based off of the number of rows and columns expected in the map file
 * INPUT:   numRows - integer number of rows in the map
 *          numCols - integer number of columns in the map
 *          maxPlayers - integer number of maxPlayers (assumed to not exceed 26 due to our implementation) 
*/
game_t* game_new(int numRows, int numCols, int maxPlayers);

/**
 * function to delete the memory allocated for a game structure
*/
void game_delete(game_t *g);

/**
 * function to fill the char** map of a game struct from mapFilename
 * returns true if succesful, false otherwise
 * INPUT:   game_t* game - game structure used
 *          char* mapFilename - filename for map.txt file
*/
bool game_fillMapFromFile(game_t *game, char *mapFilename);

/**
 * function to initialze the gold piles based off of certain parameters
 * INPUT:   game_t* game - game structure used
 *          int seed - integer seed for random number generation
 *          int GoldTotal - total amount of gold to be added to game
 *          int GoldMinNumPiles, int GoldMaxNumPiles - min and max amount of gold piles
*/
void game_initializeGold(game_t *game, int seed, int GoldTotal, int GoldMinNumPiles, int GoldMaxNumPiles);

/**
 * function to generate the start positions for players in a game
 *      game_t* game - game structure used
 *      int seed - integer seed for random number generation
*/
void game_initializePlayersStartPositions(game_t *game, int seed);

/**
 * Function to return the next player character letter
 *      Returns next letter if there is one, or else it returns '0' (character placeholder for NULL)
*/
char game_getNextLetter(game_t *game);

/**
 * Function to add a player with char* playerName and addr_t address to a game_t *game
*/
char game_addPlayer(game_t *game, const char* playerName, addr_t address);

/**
 * Function to add a spectator to the game
*/
void game_addSpectator(game_t* game, addr_t* address);

/**
 * Function to remove a spectator from the game (set pointer to null)
*/
void game_removeSpectator(game_t* game);

/**
 * Function to create and return a char* scoreTable containing the scores of all the players
 * formatted as     "[letter] [wallet] [name] \n[letter] [wallet] [name] \n ...."
*/
char* game_sendScoreTable(game_t* game);

#endif // __GAME_H