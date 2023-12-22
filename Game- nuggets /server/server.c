/**
 * Module for server in nuggets project
 * 
 * Joseph Daher, November 2023
*/
#define STATUS false // used to print status messages for when scanning a new URL or adding a word to the index

#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "../support/message.h"
#include "serverDisplay.h"
#include "serverMove.h"

/************ global constants *****************/
int seed;   // seed for RNG
const int MaxNameLength = 50;   // max number of chars in playerName
const int MaxPlayers = 26;      // maximum number of players
const int GoldTotal = 250;      // amount of gold in the game
const int GoldMinNumPiles = 10; // minimum number of gold piles
const int GoldMaxNumPiles = 30; // maximum number of gold piles

/************ glboal variables ***********************/
game_t* game; // contains the game structure, extern so it is accessible by ServerMove

/**************** function prototypes ************************/
int main(int argc, char *argv[]);
bool initializeGame(char* map);
bool validateMap(char* map, int numRows, int numCols);
bool endsWithTxt(char *str);
bool loadMap(char* mapFilename);
static bool
handleMessage(void* arg, const addr_t from, const char* message);
void sendUpdatedMap();
static void sendMapItemFunc(void* arg, const char* key, void* item);
void sendScoreToAllPlayers();
static void sendScoreItemFunc(void* arg, const char* key, void* item);

int main(int argc, char *argv[])
{
    // 	checks if there are 1 or 2 arguments
    if(argc < 2 || argc > 3){
        fprintf(stderr, "Invalid number of arguments provided. Usage: map.txt [seed]\n");
        return 1;
    }

    // 	assigns arg1:[map.txt] to a char*
    char* map=argv[1];

    // 	convert arg2:[seed] into integer
    if(argc == 3){
        seed = atoi(argv[2]);
        if(seed == 0){
            fprintf(stderr, "Error converting seed to an integer\n");
            return 1;
        }
    } else {
        // default seed value
        seed = 79;
    }

    // 	call initializeGame(), if == false, exit 1
    if(!initializeGame(map)){
        fprintf(stderr, "Error initializing game\n");
        return 1;
    } else {
        #if STATUS
        printf("Game initialized succesfully\n");
        #endif
    }
    
    // open file for logging
    FILE* fp = fopen("logserverout", "w");
    log_init(fp);
    if (fp == NULL) {
        log_e("Error in main - cannot open client log file\n");
        return 2;
    }

    // initialize network
    // initialize the message module (without logging)
    int myPort = message_init(fp);
    if (myPort == 0) {
        log_e("Error in main - cannot initialize message module");
        return 2; // failure to initialize message module
    } else {
        printf("serverPort=%d\n", myPort); // print server port
    }

    // Loop, waiting for input or for messages; provide callback functions.
    // We use the 'arg' parameter to carry a pointer to 'server'.
    message_loop(NULL, 0, NULL, NULL, handleMessage);

    // shut down the message module
    message_done();

    // free game memory
    game_delete(game);

    // close the log file
    fclose(fp);

    // exit 0
    return 0;
}

bool initializeGame(char* map)
{

    log_v("initializing game");

    // placeholder variables for map size
    int numRows = 0;
    int numCols = 0;

	// validateMap(char* map)
    // 	make sure the map it ends in .txt
    if (!endsWithTxt(map)) {
        fprintf(stderr,"Invalid map file provided: Does not end in .txt\n");
        return false;
    }

    // checks if file is openable and readable using fopen
    FILE *file = fopen(map, "r");
    if (!file) {
        fprintf(stderr, "Error opening or reading file\n");
        return false;
    }

    char line[1024]; // Assuming a line will not be longer than 1024 characters
    int lineLength = 0;
    int lineCount = 0;
    bool isFirstLine = true;

    // iterate over all lines in the file
    while (fgets(line, sizeof(line), file)) {
        if (isFirstLine) {
            // count and save number of characters in line (first line determines numCols)
            numCols = strlen(line) - 1; // -1 to not count the newline character
            isFirstLine = false;
        } else {
            // make sure each line has the same number of columns, else return error
            lineLength = strlen(line) - 1; // -1 to not count the newline character
            if (lineLength != numCols) {
                fprintf(stderr, "Error: Map is not square\n");
                fclose(file);
                return false;
            }
        }
        lineCount++;
    }

    numRows = lineCount;

    // Close the file
    fclose(file);

    // create new game structure
    game = game_new(numRows, numCols, MaxPlayers);    

	// loadMap(char* map)
    if(!loadMap(map)){
        return false;
    }

	// initializeGold
    game_initializeGold(game, seed, GoldTotal, GoldMinNumPiles, GoldMaxNumPiles);

	// wherePlayersStart
    game_initializePlayersStartLocations(game, seed);

	// return bool
    return true;
}

bool validateMap(char* map, int numRows, int numCols)
{
    // 	make sure the map it ends in .txt
    if (!endsWithTxt(map)) {
        fprintf(stderr,"Invalid map file provided: Does not end in .txt\n");
        return false;
    }

    // checks if file is openable and readable using fopen
    FILE *file = fopen(map, "r");
    if (!file) {
        fprintf(stderr, "Error opening or reading file\n");
        return false;
    }

    char line[1024]; // Assuming a line will not be longer than 1024 characters
    int lineLength = 0;
    int lineCount = 0;
    bool isFirstLine = true;

    // iterate over all lines in the file
    while (fgets(line, sizeof(line), file)) {
        if (isFirstLine) {
            // count and save number of characters in line (first line determines numCols)
            numCols = strlen(line) - 1; // -1 to not count the newline character
            isFirstLine = false;
        } else {
            // make sure each line has the same number of columns, else return error
            lineLength = strlen(line) - 1; // -1 to not count the newline character
            if (lineLength != numCols) {
                fprintf(stderr, "Error: Map is not square\n");
                fclose(file);
                return false;
            }
        }
        lineCount++;
    }

    numRows = lineCount;

    // Close the file
    fclose(file);

    // If we reach this point, the map is validated successfully
    return true;
}

// Helper function to check if a string ends with ".txt"
bool endsWithTxt(char *str)
{
    const char *extension = ".txt";
    int strLength = 0;
    
    // count the number of characters in str
    while (str[strLength] != '\0') {
        strLength++;
    }
    
    // check if the string is at least as long as the extension
    if (strLength < 4) {
        return false;
    }
    
    // compare the last four characters, return false if any characters do not match
    for (int i = 0; i < 4; i++) {
        if (str[strLength - 4 + i] != extension[i]) {
            return false;
        }
    }
    
    return true;
}

bool loadMap(char* map)
{
    // load map into game structure using helper function
    bool succesfulLoad = game_fillMapFromFile(game, map);
    if(!succesfulLoad){
        fprintf(stderr, "Error loading map from %s", map);
        return false;
    }
    return true;
}

/**************** handleMessage ****************/
/* Datagram received; print it, read a line from stdin, and use it as reply.
 * We ignore 'arg' here.
 * Return true if EOF on stdin or any fatal error.
 * 
 * Copied and adapted from miniclient.c
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{
    #if STATUS
    printf("%s\n", message);
    #endif

    // message is equal to PLAY
    if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
        // move pointer to character after PLAYER
        const char* playerName = message + strlen("PLAY ");

        // call addPlayer function
        char letter = game_addPlayer(game, playerName, from);

        // make sure letter is not null (zero)
        if(!(letter == 0)){
            // if player succesfully added, send message back to client
            char okMessage[1024];
            sprintf(okMessage, "OK %c", letter);

            #if STATUS
            printf("%s\n", message);
            #endif

            message_send(from, okMessage);

            // send "GRID nrows ncols" to new client
            char gridMessage[1024];
            sprintf(gridMessage, "GRID %d %d", game->numRows, game->numCols);
            message_send(from, gridMessage);

            // send "GOLD n p r" to new client
            char goldMessage[1024];
            sprintf(goldMessage, "GOLD 0 0 %d", nuggetsLeft());
            message_send(from, goldMessage);

        } else {
            message_send(from, "ERROR game is full\n");
        }        
    }
    // message is equal to SPECTATE
    else if (strncmp(message, "SPECTATE ", strlen("SPECTATE ")) == 0) {
        
        // remove existing spectator
        if(game->spectator != NULL){
            // send message to tell existing spectator to quit
            message_send(*game->spectator, "QUIT you have been replaced by a new spectator\n");
            // remove spectator
            game_removeSpectator(game);
        }
        
        addr_t copyFrom = from;
        addr_t* copyFromp = &copyFrom;
        game_addSpectator(game, copyFromp);

        // send "GRID nrows ncols" to new spectator
        char gridMessage[1024];
        sprintf(gridMessage, "GRID %d %d", game->numRows, game->numCols);
        message_send(from, gridMessage);
        
        // send "GOLD n p r" to new spectator
        char goldMessage[1024];
        sprintf(goldMessage, "GOLD %d", nuggetsLeft());
        message_send(from, goldMessage);
        
    }
    // message is equal to KEY
    else if (strncmp(message, "KEY ", strlen("KEY ")) == 0) {

        // move pointer to character after KEY
        char key = message[strlen("KEY ")];  // Access the character directly

        const char* address = message_stringAddr(from);
        
        // tell player/spectator to quit (data not deleted cuz unnecessary)
        if(key == 'Q' || key == 'q'){
            message_send(from, "QUIT user asked to quit \n");
        }
        else {
            // call requestMove(address, key)
            requestMove(address, key);
        }
    }
    // send ERROR message for malformed message
    else {
        message_send(from, "ERROR malformed message\n");
    }

    // send updated game to ALL players
    sendUpdatedMap();

    // check if game has ended, end loop if it has
    // Monitor the number of gold nuggets remaining; when it reaches zero,
    if(nuggetsLeft() == 0){
        // the server shall prepare a tabular summary (below), 
        // send a QUIT message (to all clients) with that summary, print the summary, and exit.
        sendScoreToAllPlayers();

        return true;
    }

    // continue looping
    return false;
}

// Function/Helper Function to iterate over all players in the game and send display message
// needs spectate_display() function
void sendUpdatedMap(){
    // iterate over list of players
    set_iterate(game->players, NULL, sendMapItemFunc);

    if(game->spectator != NULL){
        char* map = displaySpectator();
        char mapMessage[65000]; // max message size is 65000
        memset(mapMessage, 0, sizeof(mapMessage));
        sprintf(mapMessage, "DISPLAY\n%s", map);
        addr_t* spectator = game->spectator;

        message_send(*spectator, mapMessage);
        free(map);
    }
}

static void sendMapItemFunc(void* arg, const char* key, void* item) {
    player_t* player = (player_t*) item;  // Cast the void* to item_t*
    // skip NULL (deleted players)
    if (player != NULL) {
        addr_t address = player->address;
        char* map = display(player); // change back to display player
        char message[65000]; // max message size is 65507
        memset(message, 0, sizeof(message));
        sprintf(message, "DISPLAY\n%s", map);
        message_send(address, message);
        free(map); // free memory from player map after copying it to message buffer
    }
}

// Function to iterate over all players in the game and send the score table
// Assumes existence of a function game_sendScoreTable() that returns the score table as char*
void sendScoreToAllPlayers() {
    char* scoreTable = game_sendScoreTable(game); // Get the score table
    char scoreMessage[65000]; // max message size is 65507
    sprintf(scoreMessage, "QUIT GAME OVER\n%s", scoreTable);
    free(scoreTable);

    // Iterate over the list of players and send them the score table
    set_iterate(game->players, scoreMessage, sendScoreItemFunc);

    // Additionally, send the score table to the spectator if present
    if(game->spectator != NULL) {
        addr_t* spectator = game->spectator;
        message_send(*spectator, scoreMessage);
    }

    // And lastly print the score table to server
    printf("%s", scoreMessage);
}

// Helper function to send the score table to a single player
// Assumes 'arg' is a char* to the score message
static void sendScoreItemFunc(void* arg, const char* key, void* item) {
    char* scoreMessage = (char*) arg; // Cast the void* to char*
    player_t* player = (player_t*) item; // Cast the void* to player_t*

    // Skip NULL (deleted players)
    if (player != NULL && player->inGame == true) {
        addr_t address = player->address;
        message_send(address, scoreMessage);
    }
}