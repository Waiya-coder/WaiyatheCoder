/**
 * Header file for player module
 * 
 * Joseph Daher, October 2023
*/
#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../support/message.h"
#include "game.h"

/************* structure definitions ************/
typedef struct game game_t;

typedef struct player {
    char* name;           // player's name
    char letter;          // player's letter (as displayed on the map)
    int location[2];    // player location
    int** visiblePoints;    // points that have been visible to the player
    int visiblePointsLength; // number of points in visiblePoints array
    int wallet;           // holds gold collected by player
    bool inGame;           // whether player is currently in game
    addr_t address;       // player client's Internet address
} player_t;

/************* function prototypes ****************/
/**
 * Initializes and allocates memory for a new player structure in a game
 *      game_t* game - game structure used
 *      char* name - name of player
 *      addr_t addreess - address of player
 * Returns new player structure, returns NULL if player cannot be added
*/
player_t* player_new(game_t* game, const char* name, const addr_t address);

/**
 * Deletes a player and deallocates all memory associated for each player
*/
void player_delete(player_t* player, const game_t* game);

#endif // __PLAYER_H