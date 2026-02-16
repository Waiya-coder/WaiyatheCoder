/**
 * Module containing player data structure and other functions
 * Also includes location and pile data types
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../support/message.h"
#include "game.h"

/************* structure definitions ************/
// typedef struct game game_t;

// typedef struct point {
//     int x;
//     int y;
//     char symbol; // Symbol for room, wall, or boundary
// } point_t;

// typedef struct pile {
//     point_t location;
//     int gold;
// } pile_t;

// typedef struct player {
//     char* name;           // player's name
//     char letter;          // player's letter (as displayed on the map)
//     point_t* location;    // player location
//     int** visiblePoints;    // points that have been visible to the player
//     int visiblePointsLength; // number of points in visiblePoints array
//     int wallet;           // holds gold collected by player
//     bool inGame;           // whether player is currently in game
//     addr_t address;       // player client's Internet address
// } player_t;

/************* function prototypes ****************/
player_t* player_new(game_t* game, const char* name, const addr_t address);
void player_delete(player_t* player, const game_t* game);

player_t* player_new(game_t* game, const char* name, const addr_t address) {
    // first check that there is an available next letter (0 if not)
    char nextLetter = game_getNextLetter(game);
    if(nextLetter == '0'){
        return NULL;
    }

    // Allocate memory for the player structure
    player_t* player = malloc(sizeof(player_t));

    // Assign player letter
    player->letter = nextLetter;

    // Allocate and copy the player's name
    char* playerName = malloc(strlen(name) + 1);
    strcpy(playerName,name);
    player->name = playerName;
    
    // Allocate memory for the visiblePoints
    player->visiblePointsLength = 0;
    player->visiblePoints = malloc(game->numRows * game->numCols * sizeof(int*));
    for (int i = 0; i < (game->numRows * game->numCols); i++) {
        player->visiblePoints[i] = malloc(2 * sizeof(int));
    }

    // set starting location
    player->location[0] = game->startPosition[game->nextLetterIndex - 1][0];
    player->location[1] = game->startPosition[game->nextLetterIndex - 1][1];

    player->wallet = 0;
    player->inGame = true;

    // store player's address
    player->address = address;

  return player;
}

// helper function to delete a player structure
void player_delete(player_t* player, const game_t* game) {
    if (player != NULL) {
        // free the memory allocated for the name
        free(player->name);

        // free each string in the visiblePoints array
        if (player->visiblePoints != NULL) {
        for (int i = 0; i < (game->numRows * game->numCols); i++) {
            free(player->visiblePoints[i]);
        }
        // free the array of strings
        free(player->visiblePoints);
        }

        // free the player structure itself
        free(player);
    }
}