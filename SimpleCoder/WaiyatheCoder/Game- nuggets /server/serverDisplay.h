/* 
 * serverDisplay.h - header file for the serverDisplay.c file
 *
 * This file includes the functions that work
 * on the visibility and display of the map.
 * 
 * Carolina Harr, 2023
 */

#ifndef __SERVERDISPLAY_H
#define __SERVERDISPLAY_H

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

/**************** global types ****************/
/* none */

/**************** functions ****************/

/**************** isVisible ****************/
/* Checks whether a point (cx,cy) is visible from point (px, py)
 *
 * Caller provides:
 *  the x and y values of a player (px ,py)
 *  the x and y values that it wants to know whether it's visible from (px, py)
 *  all inputs are integers
 * We return:
 *  true, if (cx,cy) is visible from (px,py)
 *  false, otherwise
 * We do:
 *  check to see if something is in the way of viewing this point
 */
bool isVisible(int px, int py, int cx, int cy);

/**************** isRoomSpot ****************/
/* Checks whether a point (x,y) is a room spot
 *
 * Caller provides:
 *  the x and y values of the point it wants to know about
 *      x and y are integers
 * We return:
 *  true, if (x,y) is a room spot
 *  false, otherwise
 * We do:
 *  check game struct to see if that point has the room symbol (.)
 */
bool isRoomSpot(int x, int y);

/**************** addVisiblePoints ****************/
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
void addVisiblePoints(player_t* player);

/**************** display ****************/
/* Gets the most updated map for that player
 * 
 * Caller provides:
 *   valid pointer to player struct
 * We return:
 *   the most updated map for that player
 * We do:
 *  add the visible points to the player's visible points array
 *  create a map that has what this player can see
 */
char* display(player_t* player);

/**************** displaySpectator ****************/
/* Gets the most updated map
 * 
 * Caller provides:
 *   nothing
 * We return:
 *   the most updated map with players and gold
 * We do:
 *  add the players and gold to the map
 *  make a char* copy of the char** map
 */
char* displaySpectator();

#endif // __SERVERDISPLAY_H
