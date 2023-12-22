

/* 
 * serverMove.h - header file for CS50 serverMove.c 
 *
 * Brenda Waiya, Fall 2023
 */

#include <stdbool.h>
#include <stdio.h>
#include "serverDisplay.h"
#include "game.h"
#include "server.h"
#include "player.h"
#include "../support/message.h" 
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"

#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

typedef struct targetAndBool {
    int* target;
    bool* isPlayerP;
} targetAndBool_t;

void requestMove(const char* address, char key);

// Function to request a player's move and update the game state accordingly
// Parameters:
//   - address: Address of the player making the move
//   - key: The input key representing the move
bool requestSingleMove(player_t* player, char key);

// Function to get the target location based on the player's move key
// Parameters:
//   - game: The game object
//   - player: The player making the move
//   - key: The input key representing the move
// Returns:
//   - The target location or NULL if invalid
void get_target_location(player_t *player, char key, int target[2]);

// Function to handle a player's quit request
// Parameters:
//   - player: The player requesting to quit
void quitPlayer(player_t *player);

// Function to check if a target location is a wall
// Parameters:
//   - game: The game object
//   - x,y: The target location to check
// Returns:
//   - true if the target is a wall, false otherwise
bool isWall(game_t *game, int x, int y);

// Function to check if there is already a player at the specified target location
// Parameters:
//   - target: The target location to check
// Returns:
//   - true if there is a player at the target, false otherwise
bool alreadyPlayer(int *target);

// Callback function to check if a player is at the specified location
void isPlayerHere(void *arg, const char *key, void *item);

// Function to swap spots between two players
// Parameters:
//   - player1: The first player
//   - target: The target location for the swap
void swapSpots(player_t *player1, int *target);

// Function to find a player at a given location
// Parameters:
//   - target: The target location to check
// Returns:
//   - The player found or NULL if not found
void
whatPlayerHere(void *arg, const char *key, void *item);

// Function to check if a target location has gold
// Parameters:
//   - goldLocations: The 2D array of gold locations
//   - numRows: Number of rows in the map
//   - numCols: Number of columns in the map
//   - target: The target location to check
// Returns:
//   - true if there is gold at the target, false otherwise
int isGold(int** goldLocations, int* target);

// Function to change the gold amount at a location and update the player's gold
// Parameters:
//   - target: The target location where gold is picked up
//   - pile: The gold pile information (not used)
//   - player: The player whose gold is updated
// Function to update a player's gold and wallet
// Parameters:
//   - player: A pointer to the player whose gold and wallet need to be updated
//   - amountGold: The amount of gold to add to the player's wallet (can be negative)
void changeGold(player_t* player, int amountGold);

// Function to update a player's location
// Parameters:
//   - player: A pointer to the player whose location needs to be updated
//   - target: A pointer to the new target location
void updatePlayerLocation(player_t* player, int* target);

// Function to calculate the total number of gold nuggets left in the game
// Returns:
//   - The total number of gold nuggets remaining
int nuggetsLeft();

// Function to send a GOLD message to a player or spectator
// Parameters:
//   - n: The number of gold nuggets collected
//   - p: The total gold nuggets in the player's purse
//   - r: The remaining gold nuggets to be found
void sendGoldMessage(player_t* player, int n, int p, int r);


#endif // GAME_FUNCTIONS_H
