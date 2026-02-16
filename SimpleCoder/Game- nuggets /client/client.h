/* 
 * client.h - header file for CS50 client module
 *
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>
#include "../support/message.h"
#include "../support/log.h"

/* local structures */
// structure to store information about the client

/**************** main ****************/
/* 
 * The main function:
 * - opens the file to log messages and errors
 * - initializes message module
 * - checks command-line arguments
 * - sets address of the server
 * - sends message to server for the correct type of client (player vs spectator)
 * - initialize curses
 * - calls message_loop()
 * 
 * We return:
 * - non-zero if there is an error
 * - zero if message_loop contains no errors
 */
int main (const int argc, const char* argv[]);

/* ********* initializeCurses ********* */
/* 
 * The initializeCurses function:
 * - initializes the screen
 * - calls screenSize()
 * 
 * Is called in:
 * - main()
 * - handleMessages() under the "GRID" condition
 * 
 * We return:
 * - nothing
 */
static void initializeCurses();

/* ********* screenSize ********* */
/* 
 * The screenSize function:
 * - gets the height and width of the screen
 * - prompts the user to increase the screen size until appropiate 
 * 
 * Is called in:
 * - initializeCurses()
 * 
 * We return:
 * - nothing
 */
static void screenSize();

/* ********* handleInput ********* */
/* 
 * The handleInput function:
 * - accepts the key input from the user
 * - sends a message "KEY k" to the server, where k is the key pressed
 * - only allows the spectator to press Q or EOF (CTRL D)
 * 
 * Is called in:
 * - main()
 * 
 * We return:
 * - true if error
 * - false if no errors and we continue listening to user input
 */
static bool handleInput (void* arg);

/* ********* handleMessage ********* */
/* 
 * The handleMessage function:
 * - handles messages from the server and acts accordingly
 * 
 * Is called in:
 * - main()
 * 
 * We return:
 * - true if error
 * - false if no errors and we continue listening to the user
 */
static bool handleMessage(void* arg, const addr_t from, const char* message);
