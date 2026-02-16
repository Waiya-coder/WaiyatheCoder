/* 
 * serverDisplay.c
 *
 * see serverDisplay.h for more information.
 *
 * Carolina Harr, September 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "serverDisplay.h"
#include "server.h"
#include "../libcs50/set.h"
#include "game.h"
#include "player.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** local functions ****************/
/* not visible outside this file */

/**************** isInt ****************/
/* checks if value is an int
 * 
 * Caller provides:
 *   a double that it wants to check
 * We return:
 *   true, if int
 *   false, otherwise
 */
bool
isInt(double number)
{
  return (number == (int)number);
}

/**************** isInVisibilityArray ****************/
/* Adds visible points to the player's visible points array
 * 
 * Caller provides:
 *   valid pointer to player struct
 * We return:
 *   nothing
 * We do:
 *  loop though every point in the game struct's map array
 *  if it's visible, add it to the player's visible points array
 */
bool
isInVisibilityArray(player_t* player, int x, int y)
{
  if (player == NULL) {
    return false;
  }
  //loop through visiblePoints
  int** visiblePoints = player->visiblePoints;
  int lenVP = player->visiblePointsLength;

  int idxVP = 0; //index of visible points array
  while (idxVP < lenVP) {
    //if the point in visible points is equal to the input point
    if (visiblePoints[idxVP][0] == x && visiblePoints[idxVP][1] == y){
      return true;
    }
    idxVP++;
  }
  return false; //if get to end of loop without returning
}

/**************** global functions ****************/
/* that is, visible outside this file */
/* see serverDisplay.h for comments about exported functions */
bool isVisible(int px, int py, int cx, int cy);
bool isRoomSpot(int x, int y);
void addVisiblePoints(player_t* player);
char* display(player_t* player);
void addPlayerToMap(void *arg, const char *key, void *item);
char* mapToOneArray(char** mapArrayArray);
void addPlayerToMapIfVis(void *arg, const char *key, void *item);
char* displaySpectator();

/**************** isVisible() ****************/
/* see serverDisplay.h for description */
bool 
isVisible(int px, int py, int cx, int cy)
{
  //check if line btwn points is horizontal or vertical
  bool isHor = false;
  bool isVert = false;

  //if the x values are the same, then it's vertical
  if (px == cx){
    isVert = true;
  }

  //if the y values are the same, then it's horizontal
  if (py == cy) {
    isHor = true;
  }

  //if it's the same point, is visible
  if (isHor == true && isVert == true) {
    return true;
  }

  //find the lower x and y values
  //(makes it easier to iterate over points)
  int lessX;
  int lessY;
  int moreX;
  int moreY;
  if (px < cx) {
    lessX = px;
    moreX = cx;
  } else {
    lessX = cx;
    moreX = px;
  }
  if (py < cy) {
    lessY = py;
    moreY = cy;
  } else {
    lessY = cy;
    moreY = py;
  }
  
  //if horizontal, check points in between p and c
  if (isHor == true) {
    int currX = lessX + 1;
    while (currX <= (moreX - 1)) {
        bool roomSpot = isRoomSpot(currX, py);
        if (roomSpot == false) {
            return false;
        }
        currX ++;
    }
    //if get to end of loop before returning
    return true;
  }
  
  //if vertical, check points in between p and c
  if (isVert == true) {
    int currY = lessY + 1;
    while (currY <= (moreY - 1)) {
        bool roomSpot = isRoomSpot(px, currY);
        if (roomSpot == false) {
            return false;
        }
        currY++;
    }
    //if get to end of loop before returning
    return true;
  }

  //get equation of line between points (y=mx+b)
  double m = (double)(py - cy) / (double)(px - cx);
  double b = m*(-px) + py;

  //for diagonal lines

  //check each row in between
  int currY = lessY + 1;
  while (currY <= (moreY - 1)) {
    //check if point is on the grid
    double currX = (double)(currY - b) / (double)m;
    //if currX is an int, then it's on grid
    if (isInt(currX) == true) {
      bool roomSpot = isRoomSpot(currX, currY);
      if (roomSpot == false) {
        return false;
      }
    } else { //if point isn't on grid
      //check left
      bool leftSpot = isRoomSpot((int)floor(currX), currY);
      //check left
      bool rightSpot = isRoomSpot((int)ceil(currX), currY);
      if (leftSpot == false && rightSpot == false) {
        return false;
      }
    }
    currY++;
  }

  //check each column in between
  int currX = lessX + 1;
  while(currX <= (moreX - 1)) {
    //check if point is a point on the grid
    double currY2 = m*currX + b;

    //if currY is an int, then it's on grid
    if (isInt(currY2) == true) {
      bool roomSpot = isRoomSpot(currX, currY2);
      if (roomSpot == false) {
        return false;
      }
    } else {
      //check below
      bool belowSpot = isRoomSpot(currX, (int)ceil(currY2));
      //check above
      bool aboveSpot = isRoomSpot(currX, (int)floor(currY2));
      if (belowSpot == false && aboveSpot == false) {
        return false;
      }
    }
    currX++;
  }
  return true; //if didn't exit earlier
}

/**************** isRoomSpot() ****************/
/* see serverDisplay.h for description */
bool 
isRoomSpot(int x, int y)
{
  char** map = game->map;
  char spot= map[y][x];
  char roomSpot = '.';

  if (spot == roomSpot) {
    return true;
  }
  return false;
}


/**************** addVisiblePoints() ****************/
/* see serverDisplay.h for description */
void 
addVisiblePoints(player_t* player)
{
  if (player == NULL) {
    return;
  }

  int** visiblePoints = player->visiblePoints; 
  int lenVP = player->visiblePointsLength; 
  const int currX = player->location[0];
  const int currY = player->location[1]; 
  
  int nRows = game->numRows; //get number of rows of map
  int nCols = game->numCols; //get number of rows of map
  //iterate through each line in the game map
  for (int j = 0; j < (nRows); j++) {
    //iterate through the characters in each line
    for (int i = 0; i < (nCols); i++) {
      if (isVisible(currX, currY, i, j) == true) {
        //check if it's already in visibility array
        if (isInVisibilityArray(player, i, j) == true) {
          //do nothing
        } else {
          //add it to visibility array
          lenVP = player->visiblePointsLength;
          visiblePoints[lenVP][0] = i;
          visiblePoints[lenVP][1] = j;

          lenVP++;

          player->visiblePointsLength = lenVP;

        }
      }
    }
  }
}

//struct for iterating through player set
typedef struct mapAndPlayer {
  char** map;
  player_t* currPlayer;
} mapAndPlayer_t;

/**************** display() ****************/
/* see serverDisplay.h for description */
char* 
display(player_t* player)
{
  if (player == NULL) {
    return NULL;
  }

  //add the current visible points to the array of points the player has seen
  addVisiblePoints(player);

  //get map and number of rows and columns from the game struct
  char** gameMap = game->map; //get map
  int nRows = game->numRows; //get number of rows of map
  int nCols = game->numCols; //get number of columns of map
  

  //malloc an array of nRows strings
  char** mapArrayArray = (char**)malloc(nRows * sizeof(char*));

  if (mapArrayArray == NULL) {
    return NULL;
  }

  //allocate memory for each row in the array to be nCols spaces
  for (int i = 0; i < nRows; i++){
    mapArrayArray[i] = (char*)malloc((nCols + 1) * sizeof(char)); //+1 for null terminator
    if (mapArrayArray[i] == NULL) {
      return NULL;
    }

    //fill in the string with spaces
    for (int j = 0; j < nCols; j++) {
      mapArrayArray[i][j] = ' ';
    }
    //null terminate the string
    mapArrayArray[i][nCols] = '\0';
  }

  //for all visible spots, grab the char from the game map and put into new map
  int** visiblePoints = player->visiblePoints; 
  int lenVP = player->visiblePointsLength;

  // for (int i = 0; i < lenVP; i++){
  //   printf("%d ", i);
  //   printf("%d ", visiblePoints[i][0]);
  //   printf("%d \n", visiblePoints[i][1]);
  // }

  for (int i = 0; i < lenVP; i++) {
    int x = visiblePoints[i][0];
    int y = visiblePoints[i][1];

    //grab that char from the game map
    char getMapChar = gameMap[y][x];

    //put this char into new map
    mapArrayArray[y][x] = getMapChar;
  }

  //get gold array
  int** gold = game->goldLocations;
  //get length of gold array
  int goldLen = game->numGoldPiles;

  //loop through all the points where gold is
  for (int i = 0; i < goldLen; i++) {
    int x = gold[i][0]; //x value of this point
    int y = gold[i][1]; //y value of this point
    int amt = gold[i][2];
    
    if (amt != 0) {
      //if that point is visible, add gold to map
      bool isVis = isVisible(player->location[0], player->location[1], x, y);
      if (isVis == false) {
        //if not visible, don't do anything
      } else {
        //put in gold symbol
        mapArrayArray[y][x] = '*';
      }
    }
    
  }
  
  //get player set
  set_t* allPlayers = game->players;

  //make mapAndPlayer type for iterating
  mapAndPlayer_t* myMapAndPlayer = malloc(sizeof(mapAndPlayer_t));
  myMapAndPlayer->map = mapArrayArray;
  myMapAndPlayer->currPlayer = player;

  //loop through all the points where players are
  //if that point is visible, add player and their char to map
  set_iterate(allPlayers, myMapAndPlayer, addPlayerToMapIfVis);

  //wherever player is at, put @
  mapArrayArray[player->location[1]][player->location[0]] = '@';

  // joe change
  free(myMapAndPlayer);

  //turn mapArrayArray, which is an array of arrays, into one array
  return mapToOneArray(mapArrayArray);

}

/**************** addPlayerToMapIfVis ****************/
/* if where other player is is visible to current player, add to map
 * 
 * Caller provides:
 *   struct holding map and current player
 *   key of other player
 *   item of other player
 * We return:
 *   nothing
 */
void 
addPlayerToMapIfVis(void *arg, const char *key, void *item)
{
  if (arg == NULL || key == NULL || item == NULL) {
    return;
  }

  //grab myMapAndPlayer type
  mapAndPlayer_t* myMapAndPlayer = (mapAndPlayer_t*)arg;

  //grab the current player from arg
  player_t* currPlayer = (player_t*)(myMapAndPlayer->currPlayer);

  //grab current map from arg
  char** mapArrayArray = (char**)(myMapAndPlayer->map);

  //grab player we are iterating on
  player_t* iterPlayer = (player_t*) item;

  //if that player is visible to the current player
  bool isVis = isVisible(currPlayer->location[0], currPlayer->location[1], iterPlayer->location[0], iterPlayer->location[1]);
    if (isVis == false) {
      //if not visible, don't do anything
    } else {
      //put in this player's symbol
      mapArrayArray[iterPlayer->location[1]][iterPlayer->location[0]] = iterPlayer->letter;
    }
}

char*
mapToOneArray(char** mapArrayArray)
{
  if (mapArrayArray == NULL) {
    return NULL;
  }

  int m = game->numCols;
  int n = game->numRows;
  
  //malloc an array that will hold (numCols+1) * (numRows)
  char* newMap = malloc((n * (m + 1) + 1));

  //joe change
  // int size = (n * (m + 1) + 1);
  memset(newMap, 0, (n * (m + 1) + 1));
  
  //get number of rows and columns of map
  int nRows = game->numRows; 
  int nCols = game->numCols;
  
  //current index of newMap
  int idxNM = 0;

  //iterate through old map adding each line to new map 
  for (int i = 0; i < nRows; i++){
    for (int j = 0; j < nCols; j++){
      newMap[idxNM] = mapArrayArray[i][j];
      idxNM++;
    }
    //each time you get to end of a line, add a $
    newMap[idxNM] = '$';
    idxNM++;
  }

  // free mapArrayArray
  for (int i = 0; i < nRows; i++){
    free(mapArrayArray[i]);
  }
  free(mapArrayArray);

  return newMap;
}

char*
displaySpectator()
{
  //make a copy of the game map
  char** gameMap = game->map; //get game map
  int nRows = game->numRows; //get number of rows of map

  //allocate memory for spectator's version of the map
  char** specMap = (char**)malloc(nRows * sizeof(char*));

  //check if memory allocation was successful
  if (specMap == NULL) {
    return NULL;
  }

  //copy each string from the og array
  for (int i = 0; i < nRows; i++) {
    //make sure gameMap string isn't null
    if (gameMap[i] != NULL) {
      //allocate memory for the copied string
      specMap[i] = (char*)malloc((strlen(gameMap[i]) + 1) * sizeof(char));

      //check if memory allocation was successful
      if (specMap[i] == NULL) {
        return NULL;
      }

      //copy content of gameMap into specMap
      strcpy(specMap[i], gameMap[i]);
    } else {
      return NULL;
    }
  }

  //add gold onto map
  //get length of gold array
  int goldLen = game->numGoldPiles;

  //loop through all the points where gold is
  for (int i = 0; i < goldLen; i++) {
    int x = game->goldLocations[i][0]; //x value of this point
    int y = game->goldLocations[i][1]; //y value of this point
    int amt = game->goldLocations[i][2]; //amount of gold at this point
    
    if (amt != 0){
      //add gold to map
      specMap[y][x] = '*';  
    }
    
  }

  //add players onto map

  //get player set
  set_t* allPlayers = game->players;

  //loop through all the points where players are
  set_iterate(allPlayers, specMap, addPlayerToMap);

  //turn map from char** into char*
  //and return
  return mapToOneArray(specMap);
}

void
addPlayerToMap(void *arg, const char *key, void *item)
{
  if (arg == NULL || key == NULL || item == NULL) {
    return;
  }

  //grab map from arg
  char** specMap = (char**)arg;

  //grab player we are iterating on
  player_t* iterPlayer = (player_t*)item;

  //add player to map
  specMap[iterPlayer->location[1]][iterPlayer->location[0]] = iterPlayer->letter;
}