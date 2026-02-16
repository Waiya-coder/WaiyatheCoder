/**
 * Module for game structure (used by Server to store the game set)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "player.h"
#include "../libcs50/set.h"
#include "../support/message.h" // for message_stringAddr

/******** function prototypes ********/
game_t* game_new(int numRows, int numCols, int maxPlayers);
void game_delete(game_t *g);
static void player_delete_helper(void* arg, const char* key, void* item);
bool game_fillMapFromFile(game_t *game, char *mapFilename);
void game_initializePlayersStartLocations(game_t* game, int seed);
void game_initializeGold(game_t *game, int seed, int GoldTotal, int GoldMinNumPiles, int GoldMaxNumPiles);
char game_getNextLetter(game_t *game);
char game_addPlayer(game_t *game, const char* playerName, addr_t address);
void game_addSpectator(game_t* game, addr_t* address);
void game_removeSpectator(game_t* game);
static bool isValidStartTile(game_t *game, int row, int col);
static bool isValidGoldSpot(game_t* game, int row, int col);
char* game_sendScoreTable(game_t* game);
static void getWalletItemFunc(void* arg, const char* key, void* item);

// function to create a new empty game structure
game_t* game_new(int numRows, int numCols, int maxPlayers) {
    game_t* g = malloc(sizeof(game_t));

    // Allocate memory for the map, which is an array of char pointers
    g->map = malloc(numRows * sizeof(char*)); // no null terminator needed since number of rows is known

    // Initialize each row of the map
    for (int i = 0; i < numRows; ++i) {
        g->map[i] = malloc((numCols + 1) * sizeof(char)); // +1 for null-terminator
    }

    // store number of rows and columns, and max players in game structure
    g->numRows = numRows;
    g->numCols = numCols;
    g->maxPlayers = maxPlayers;

    // Initialize goldLocations
    g->goldLocations = NULL;

    // Initialize players
    g->players = set_new();

    // Initialize other members of g as needed
    g->inGame = true;
    g->spectator = NULL; //malloc(sizeof(addr_t));

    return g;
}

void game_delete(game_t *g) {
    if (g != NULL) {

        // Free gold locations
        if (g->goldLocations != NULL) {
            for (int i = 0; i < g->numGoldPiles; i++) {
                free(g->goldLocations[i]);
            }
            free(g->goldLocations);
        }

        // free each string in the map array
        for (int i = 0; i < g->numRows; ++i) {
            free(g->map[i]);
        }
        // free the map array itself
        free(g->map);

        // free set of players
        set_iterate(g->players, (void*)g, player_delete_helper);
        set_delete(g->players,NULL); // add player_delete function later

        // Finally, free the game structure itself
        free(g);
    }
}

static void player_delete_helper(void* arg, const char* key, void* item){
    game_t* game = arg;
    player_t* player = item;
    player_delete(player,game);
}

// helper function to fill a char** map in the game structure from a map filename
bool game_fillMapFromFile(game_t *game, char *mapFilename) {
    // open file
    FILE *file = fopen(mapFilename, "r");
    if (!file) {
        return false;
    }

    char buffer[game->numCols + 2]; // +2 for newline and null terminator
    int row = 0;

    while (fgets(buffer, sizeof(buffer), file) && row < game->numRows) {
        // remove any possible newline character at the end of the line
        buffer[strcspn(buffer, "\n")] = 0;

        // check if the line length matches expected column count
        if (strlen(buffer) != game->numCols) {
            fclose(file);
            return false;
        }

        // copy the buffer to the map row
        strncpy(game->map[row], buffer, game->numCols);
        // game->map[row][game->numCols] = '\n'; // ensure newline at end of line
        game->map[row][game->numCols] = '\0'; // ensure null termination

       // printf("%s\n",game->map[row]);

        row++;
    }

    fclose(file);

    // check if we filled the expected number of rows
    if (row != game->numRows) {
        return false;
    }

    // initialize player letters and index pointer
    for (int i = 0; i < 26; ++i) {
        // 'a' has ASCII value of 97
        game->playerLetters[i] = 'a' + i;
    }
    game->nextLetterIndex = 0;

    return true;
}

// function to fill the start positions array of a game
void game_initializePlayersStartLocations(game_t *game, int seed) {
    // seed the random number generator using the seed
    srand(seed);

    for (int i = 0; i < game->maxPlayers; i++) {
        bool positionFound = false;

        while (!positionFound) {
            // generate a random position within the bounds of the map
            int randRow = rand() % game->numRows;
            int randCol = rand() % game->numCols;

            // check if the generated position is valid
            if (isValidStartTile(game, randRow, randCol)) {
                game->startPosition[i][1] = randRow; // y-location (row)
                game->startPosition[i][0] = randCol; // x-location (column)

                // uncomment to check start positions
                // printf("%d start position: %d %d\n",i,game->startPosition[i][0],game->startPosition[i][1]);

                positionFound = true;
            }
        }
    }
}

// helper function to check if the map tile is a valid start tile (is a '.')
static bool isValidStartTile(game_t *game, int row, int col) {
    if(row > game->numRows || col > game->numCols){
        return false;
    }

    // Iterate over goldLocations to check if the tile coincides with a gold location
    for (int i = 0; i < game->numGoldPiles; i++) {
        if (game->goldLocations[i][0] == col && game->goldLocations[i][1] == row) {
            // If the tile matches a gold location, it's not a valid start tile
            return false;
        }
    }

    return game->map[row][col] == '.';  // Assuming '.' denotes a valid starting position
}

// Function to initialize gold piles in the game
void game_initializeGold(game_t *game, int seed, int GoldTotal, int GoldMinNumPiles, int GoldMaxNumPiles) {
    srand(seed); // Seed the random number generator

    int numPiles = GoldMinNumPiles + rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1);
    game->goldLocations = malloc(numPiles * sizeof(int*)); // Allocate memory for gold piles pointers
    if (game->goldLocations == NULL) {
        //allocation failed
        return;
    }

    game->numGoldPiles = numPiles;

    for (int i = 0; i < numPiles; i++) {
        game->goldLocations[i] = malloc(3 * sizeof(int)); // Allocate memory for x,y,amount for each pile
        if (game->goldLocations[i] == NULL) {
            //allocation failed
            return;
        }
    }

    int goldRemaining = GoldTotal;
    
    int goldLen2 = 0;
    for (int i = 0; i < numPiles; i++) {
        bool placed = false;

        while (!placed) {
            int row = rand() % game->numRows;
            int col = rand() % game->numCols;

            if (isValidGoldSpot(game, row, col)) {
                // randomly generate size of gold pile and call addGoldPile
                if ((numPiles - i) == 0) {
                  continue;
                }
                int baseGold = (goldRemaining*2 / (numPiles-i));
                int goldValue;

                if (i == numPiles - 1) {
                    goldValue = goldRemaining;
                } else {
                    int randomPart = 0;
                    if ((baseGold - numPiles + i) == 0) {
                      randomPart = 1;
                    } else {
                      randomPart = 1 + rand() % (baseGold - numPiles + i);
                    }
                    goldValue = randomPart;
                    goldRemaining -= goldValue; // Update goldRemaining for the next iteration
                }

                game->goldLocations[i][0] = col;     // x (col)
                game->goldLocations[i][1] = row;     // y (row)
                game->goldLocations[i][2] = goldValue; // Amount of gold

                // uncomment to check gold positions
                // printf("Gold in %d %d: %d \n",game->goldLocations[i][0],game->goldLocations[i][1],game->goldLocations[i][2]);

                placed = true;
            }
        }
        goldLen2++;
    }
    game->numGoldPiles = goldLen2;
}

// helper function to check if tile is a valid gold spot (a ".")
static bool isValidGoldSpot(game_t* game, int row, int col) {
    if(row > game->numRows || col > game->numCols){
        return false;
    }
    return game->map[row][col] == '.';  // '.' denotes a valid empty room tile
}

// function to return next available game letter
char game_getNextLetter(game_t *game){
    char letter = '0';
    if(game->nextLetterIndex < 26){
        letter = game->playerLetters[game->nextLetterIndex];
        game->nextLetterIndex++;
    }
    return letter;
}

char game_addPlayer(game_t *game, const char* playerName, addr_t address)
{
    // create new player object from parameters
    player_t* player = player_new(game, playerName, address);

    // check if player is NULL, return 0 on error (player cannot be initialized)
    if(player == NULL){
        return '0';
    }

    const char* addressAsChar = message_stringAddr(address);

    set_insert(game->players, addressAsChar, player);
    return player->letter;
}

void game_addSpectator(game_t* game, addr_t* address){
    // memcpy(game->spectator, address, sizeof(addr_t)); 
    if(address != NULL){
        game->spectator = address;
    }
}

void game_removeSpectator(game_t* game){
    game->spectator = NULL;
}

char* game_sendScoreTable(game_t* game)
{
    // Assuming maxPlayers is the maximum number of players
    int maxPlayers = game->maxPlayers;
    int bufferSize = maxPlayers * 70;  // Adjust 70 based on above calculation

    char* message = malloc(bufferSize);
    memset(message, 0, bufferSize);

    if (message == NULL) {
        // Handle allocation failure
        return NULL;
    }

    set_iterate(game->players, message, getWalletItemFunc);

    return message; // Caller must free this memory
}

static void getWalletItemFunc(void* arg, const char* key, void* item)
{
    player_t* player = (player_t*) item;
    char* message = (char*) arg;

    if (player != NULL) {
        char playerInfo[70];  // temporary buffer for each player's info
        snprintf(playerInfo, sizeof(playerInfo), "%c %d %s\n", player->letter, player->wallet, player->name);

        // append playerInfo to message
        strcat(message, playerInfo);
    }
}