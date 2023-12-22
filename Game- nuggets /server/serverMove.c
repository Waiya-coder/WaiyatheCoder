/* 
 * serverMove.c
 * 
 * 
 *
 * see serverMove.h for more information.
 *
 * Brenda Waiya and Carolina Harr, Fall 2023
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include "serverDisplay.h"
#include "game.h"
#include "server.h"
#include "player.h"
#include "../support/message.h"
#include "../libcs50/set.h"
#include "../libcs50/hashtable.h"

//declaring all functions at the top
void requestMove(const char* address, char key);
bool requestSingleMove(player_t* player, char key);
void get_target_location(player_t *player, char key, int target[2]);
void quitPlayer(player_t *player);
bool isWall(game_t *game, int x, int y);
bool alreadyPlayer(int* target);
void isPlayerHere (void* arg, const char* key, void* item);
void swapSpots(player_t *player1, int* target);
void whatPlayerHere(void *arg, const char *key, void *item);
int isGold(int** goldLocations, int* target);
void changeGold(player_t* player, int amountGold);
void updatePlayerLocation(player_t* player, int* target);
int nuggetsLeft();
void sendGoldMessage(player_t* player, int n, int p, int r);

bool 
isUpperCase(char ch)
{
  return (ch >= 'A' && ch <= 'Z');
}

void 
requestMove(const char* address, char key)
{
  //get the player from the address
  player_t* player = (player_t*)set_find(game->players, address);
  
    // chevk if player is NULL (if address is spectator)
    if(player==NULL){
        return;
    }

  bool keepLooping = true;
  while (keepLooping == true) {
    bool isMoveWall = requestSingleMove(player, key);
    
    //if the key is lower case, only move once
    if (isUpperCase(key) == false) {
      keepLooping = false;
    }

    //if the single move ran into a wall (or other bad space), stop moving
    if (isMoveWall == true) {
      keepLooping = false;
    }
    sendUpdatedMap();
  }
}

bool 
requestSingleMove(player_t* player, char key)
{
    // Get the target location based on the key
    int target[2];
    get_target_location(player, key, target);

    // uncomment this for verbose player movement logging
    // printf("Player %c is at %d %d, and wants to go to %d %d\n",player->letter,player->location[0],player->location[1],target[0],target[1]);

    // Check if the target location is a wall
    bool isTargetWall = isWall(game, target[0], target[1]);
    if (isTargetWall == true) {
        return true;
    }
    // Check if there is already a player at the target location
    bool isAlrPlayer = alreadyPlayer(target);
    if (isAlrPlayer == true) {
        // Swap spots between players
        swapSpots(player, target);
        return false;
    }
    
    // Check if there is gold at the target location
    int** goldLocations = game->goldLocations;
    int isTargetGold = isGold(goldLocations,target);
    if (isTargetGold != 0) {
        // Change the gold amount and update player's gold
        changeGold(player, isTargetGold);
        int nuggetsLeftNow = nuggetsLeft();
        sendGoldMessage(player, isTargetGold, player->wallet,nuggetsLeftNow);
    }
    // Update the player's location
    updatePlayerLocation(player, target);

    return false;
}

void get_target_location(player_t *player, char key, int target[2])
{

    // Get player's coordinates
    int x = player->location[0];
    int y = player->location[1];

    // Handle each direction of movement
    if (key == 'Q')
    {
        // Handle quit keystroke
        quitPlayer(player); // Call quitPlayer function
    }
    else if (key == 'h' || key == 'H') {
         target[0] = x - 1;
         target[1] = y;
    
    }
    else if (key == 'l' || key == 'L'){
      target[0] = x + 1;
      target[1] = y;
    }

    else if (key == 'j' || key == 'J')
    {
        target[0] = x;
        target[1] = y + 1;
    }
    else if (key == 'k' || key == 'K')
    {
        target[0] = x;
        target[1] = y - 1;
    }
    else if (key == 'y' || key == 'Y')
    {
        target[0] = x - 1;
        target[1] = y - 1;
    }
    else if (key == 'u' || key == 'U')
    {
        target[0] = x + 1;
        target[1] = y - 1;
    }
    else if (key == 'b' || key == 'B')
    {
        target[0] = x - 1;
        target[1] = y + 1;
    }
    else if (key == 'n' || key == 'N')
    {
        target[0] = x + 1;
        target[1] = y + 1;
    }
}

// Function to handle quit keystroke
void quitPlayer(player_t *player)
{
    player->inGame = false;
    char* message = "QUIT user asks to quit"; 
    message_send(player->address, message);
}

// Function to check if there is already a player in the specified location
bool isWall(game_t *game, int x, int y)
{
    // Validate parameters
    if (game == NULL)
    {
        return true; // Treat as a wall if game or target is invalid
    }

    // Check if the location is within the bounds of the map
    if (x >= 0 && x < game->numCols && y >= 0 && y < game->numRows)
    {
        // Check if the character at the target location in the map is a wall character
        char mapCharacter = game->map[y][x];
        
        if (mapCharacter == '-' || mapCharacter == '|' || mapCharacter == '+' || mapCharacter == ' ')
        {
            return true; // It's a wall
        } else {
            return false;
        }
    } else {
        return true;
    }
}

bool 
alreadyPlayer(int* target)
{
  if (target == NULL) {
    return false;
  }
  bool isPlayer = false;
  bool* isPlayerp = &isPlayer;
  if (isPlayerp == NULL) {
    return false;
  }

  targetAndBool_t* myTargetAndBool = malloc(sizeof(targetAndBool_t));
  if (myTargetAndBool == NULL) {
    return false;
  }

  myTargetAndBool->target = target;
  myTargetAndBool->isPlayerP = isPlayerp;
  
  set_iterate(game->players,myTargetAndBool,isPlayerHere);

  bool* afterIsPlayerp = myTargetAndBool->isPlayerP;
  if (afterIsPlayerp == NULL) {
    return false;
  }
  bool afterIsPlayer = (*afterIsPlayerp);

  free(myTargetAndBool);
  return afterIsPlayer;
}

void 
isPlayerHere (void* arg, const char* key, void* item)
{   
    if (arg == NULL || key == NULL || item == NULL) {
      return;
    }
    targetAndBool_t* myTAB = (targetAndBool_t*) arg;
    int* target = myTAB->target;
    int targetX = target[0];
    int targetY = target[1];

    if (target == NULL || myTAB->isPlayerP == NULL) {
      return;
    }
    
    player_t* currPlayer = (player_t*)item;
    int currX = currPlayer->location[0];
    int currY = currPlayer->location[1];
    if (targetX == currX && targetY== currY){
        bool isPlayer = true;
        myTAB->isPlayerP = &isPlayer;
    } else {
        //don't change isPlayerP
    }
}

typedef struct twoPlayers {
    player_t* player1;
    player_t* player2;
} twoPlayers_t;

// Function to swap spots between players
void swapSpots(player_t *player1, int *target)
{
    // get the player location
    int *player_location = player1->location;
    int playerOGX = player_location[0];
    int playerOGY = player_location[1];

    int player_locationOG[2];
    player_locationOG[0] = playerOGX;
    player_locationOG[1] = playerOGY;

    // Update the player's location
    updatePlayerLocation(player1, target);

    player_t* player2 = NULL;
    twoPlayers_t* twoPlayers = malloc(sizeof(twoPlayers_t));
    twoPlayers->player1 = player1;
    twoPlayers->player2 = player2;

    //find the player that is currently at target
    set_iterate(game->players, twoPlayers, whatPlayerHere);
    if (twoPlayers->player2 == NULL) {
        return;
    }

    //update swapping player's location
    updatePlayerLocation(twoPlayers->player2, player_locationOG);

    free(twoPlayers);

}

void
whatPlayerHere(void *arg, const char *key, void *item)
{
  //get two players
  twoPlayers_t* twoPlayers = (twoPlayers_t*)arg;
  player_t* player1 = twoPlayers->player1;

  //get player from item
  player_t* comPlayer = (player_t*)item;// player we're comparing with player1

  if (player1->location[0] == comPlayer->location[0] && player1->location[1] == comPlayer->location[1]) {
    //if they're equal, set comPlayer equal to player2
    twoPlayers->player2 = comPlayer;
  }
}

//checks if there is gold at this point
//if there is, it changes the amount of gold there to zero
//returns the amount of gold there was there
int isGold(int** goldLocations, int* target) 
{
    //grab gold locations from game struct
    int** gold = game->goldLocations;
    //get lenth of gold array
    int goldLen = game->numGoldPiles;

    //check for null points
    if (gold == NULL) {
        return false; // Return false if either argument is NULL
    }

    // Get the x and y coordinates from the target point
    int targetX = target[0];
    int targetY = target[1];


    for (int i = 0; i < goldLen; i++) {
        int goldx = gold[i][0]; //x value of this point
        int goldy = gold[i][1]; //y value of this point
        if (targetX == goldx && targetY == goldy) {
            int temp = gold[i][2];
            gold[i][2] = 0;
            return temp;
        }
    }
    return 0;
}

// update player's gold
void 
changeGold(player_t* player, int amountGold)
{
    player->wallet += amountGold;
} 

void updatePlayerLocation(player_t* player, int* target)
{
    //player's location
    player->location[0] = target[0]; 
    player->location[1] = target[1];
}

int
nuggetsLeft()
{
  //variable that holds now much of the nuggets are left (that we've calculated so far)
  int currNugs = 0;

  //grab gold locations from game struct
  int** gold = game->goldLocations;
  //get lenth of gold array
  int goldLen = game->numGoldPiles;

  //iterate over goldLocations
  if (gold == NULL) {
    return 0;
  }

  for (int i = 0; i < goldLen; i++) {
    int goldAmt = game->goldLocations[i][2]; // amount of gold in this pile
    currNugs+=goldAmt;
  }

  return currNugs;
}

void 
sendGoldRemain(void *arg, const char *key, void *item)
{
  //grab from parameters and type caste
  char messageRemain[100];
  snprintf(messageRemain, sizeof(messageRemain), "%s", (char*)arg);
  player_t* player = (player_t*)item;

  message_send(player->address, messageRemain);

}

void
sendGoldMessage(player_t* player, int n, int p, int r)
{
  //tell the other players that someone collected gold
  char messageRemain[100];
  snprintf(messageRemain, sizeof(messageRemain), "GOLD %d", r);
  set_iterate(game->players, messageRemain, sendGoldRemain);

  //tell the player they collected cold
  char message[100];
  snprintf(message, sizeof(message), "GOLD %d %d %d", n, p ,r);
  message_send(player->address, message);

  //tell spectator how much there is remaining
  if (game->spectator != NULL) {
    message_send(*(game->spectator), messageRemain);
  }
  
}

    