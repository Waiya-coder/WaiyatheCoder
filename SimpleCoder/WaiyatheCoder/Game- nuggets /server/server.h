/**
 * Header file for server module for Nuggets Project
 * 
 * Joseph Daher, November 2023
*/
#ifndef __SERVER_H
#define __SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "../support/message.h"
#include "serverDisplay.h"
#include "serverMove.h"

/*************** global variables *************/
extern game_t* game; // global structure to hold the game

/************ global constants *****************/
extern int seed;   // seed for RNG
extern const int MaxNameLength;   // max number of chars in playerName
extern const int MaxPlayers;      // maximum number of players
extern const int GoldTotal;      // amount of gold in the game
extern const int GoldMinNumPiles; // minimum number of gold piles
extern const int GoldMaxNumPiles; // maximum number of gold piles

/**************** function prototypes *********************/
int main(int argc, char *argv[]);
bool initializeGame(char* map);
bool validateMap(char* map, int numRows, int numCols);
bool initializeGame(char* map);
bool endsWithTxt(char *str);
bool loadMap(char* map);
void sendUpdatedMap();
void sendScoreToAllPlayers();

/**
 * Main function
 * Expects one or two arguments when executed from the command line as follows:
 *      ./server map.txt [seed]
 * 
 * where map.txt is is a valid nuggets map file in ~.txt format
 *  and [seed] is an optional parameter for an integer seed for random number generation
*/
int main(int argc, char *argv[]);

/**
 * Function to initialize the game (calls other helper functions)
 * INPUT: map - map filename
*/
bool initializeGame(char* map);

/**
 * Function to validate the map from a filename (make sure it is openable and readable, and is a square size) and calculate
 * the number of lines and number of columns in the map (passed and incremented as arguments)
*/
bool validateMap(char* map, int numRows, int numCols);

/**
 * Function to initialize the empty game structure and load it with the map
*/
bool initializeGame(char* map);

/**
 * Helper function to check if char* string ends with ".txt"
 * Returns true if char* ends with ".txt", false otherwise
*/
bool endsWithTxt(char *str);

/**
 * Loads the map into the game structure global variable
*/
bool loadMap(char* map);

/**
 * Helper function to send an map message to all players (uses ServerDisplay)
 *      iterates over set of players in the game using sendMapItemFunc
*/
void sendUpdatedMap();

/**
 * Halper function to send a score message to all players
 *      iterates over set of players in the game using sendScoreItemFunc
*/
void sendScoreToAllPlayers();

#endif // __SERVER_H