/* 
 * client.c
 * 
 * ./client hostname port [playername]
 *
 * see client.h for more information.
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>
#include "client.h"
#include "../support/message.h"
#include "../support/log.h"

// from ncurses.c
#define CTRL(c)  ((c)-0100)  // control character

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/

// structure to store information about the client
typedef struct client {
    char letter; // received after "OK k" from server
    int purse; // amount of gold in player's bag
    int now_collect; // amount of gold player just collected
    int remain; // how much gold remains in the game overall
    bool isSpec; // which type of client (player or spectator)
    addr_t server;
    const char* hostname;
    const char* port;
    int G_NROWS; // row in grid of grid
    int G_NCOLS; // column in grid of grid
} client_t;

/* Declaration of local types */
static client_t client;

/**************** global types ****************/
/* none */

/* Local function prototypes */
int main (const int argc, const char* argv[]);
static void initializeCurses();
static void screenSize();
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);

/**************** local functions ****************/
/* not visible outside this file */

/* ***************** main ********************** */
int
main (const int argc, const char* argv[]) {

    // create file to log into
    FILE* fp = fopen("logclientout.txt", "w");
    log_init(fp);
    if (fp == NULL) {
        log_e("Error in main - cannot open client log file\n");
        return 2;
    }

    // initialize message module and logs to stderr
    if (message_init(fp) == 0) {
        log_e("Error in main - cannot init message module\n");
        return 2;
    }

    // check if arguments are valid 
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Error in main - Usage: %s hostname port ['playername']\n", argv[0]);
        log_e("Error in main - invalid commandline inputs\n");
        return 2;
    }

    // uses command to create address for server 
    const char* hostname = argv[1];
    const char* port = argv[2];
    addr_t server;
    if (!message_setAddr(hostname, port, &server)) {
        fprintf(stderr, "Error in main - cannot set address from %s %s\n", hostname, port);
        log_s("Error in main - cannot set address from %s\n", port);
        return 2;
    }
    client.server = server;
    client.hostname = hostname;
    client.port = port;

    // check if player or spectator
    if (argc == 4) { // client is a player
        client.isSpec = false;
        char message[40];
        sprintf(message, "PLAY %s", argv[3]);
        message_send(client.server, message);
    }
    else if (argc == 3) { // client is a spectator
        client.isSpec = true;
        message_send(client.server, "SPECTATE ");
    }

    // initialize ncurses and checks for appropriate screen size
    initializeCurses(); 

    // loop and handle messages & inputs
    bool success = message_loop(&client.server, 0, NULL, handleInput, handleMessage);

    // end the message module
    message_done();

    // close the log file
    fclose(fp);

    return success? 0 : 1;
}

/* ********* initializeCurses ********* */
/* see client.h for description */
static void
initializeCurses() {
    // initializes the screen
    initscr();

    cbreak();
    noecho();

    // set colors for the board
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    log_v("Initialized curses\n");

    // prompt user to change screen to acceptable size
    screenSize();
}

/* ********* screenSize ********* */
/* see client.h for description */
static void 
screenSize() {
    // from ncurses.c
    // get the boundaries of the screen
    int ly, lx; // upper left
    getbegyx(stdscr, ly, lx);
    int uy, ux; // lower right
    getmaxyx(stdscr, uy, ux);

    // get height and width of screen
    int height = uy - ly;
    int width = ux - lx;

    // log_d("GAME height: %d", client.G_NROWS);
    // log_d("GAME width: %d", client.G_NCOLS);

    // loops through until height is more than NROWS+1 and NCOLS+1
    while ((client.G_NROWS) + 1 > height || (client.G_NCOLS) + 1 > width) {
        log_v("Telling client to increase screen size");
        clear();

        // from lifeconway game C file
        mvprintw(0, 0, "Invalid in screenSize - increase your window to at least %d x %d", client.G_NROWS, client.G_NCOLS);
        refresh();

        // sleep for a second
        napms(1000);

        // recalculate screen
        getmaxyx(stdscr, uy, ux);
        height = uy - ly;
        width = ux - lx;

    }

    // log_d("Grid height: %d", height);
    // log_d("Grid width: %d", width);

}


/* ********* handleInput ********* */
/* see client.h for description */
static bool 
handleInput(void* arg) {

    // input character from users
    int c;
    c = getch();
    char message[20];

    // if client is spectator
    if (client.isSpec) {
        switch(c) {
            case 'Q': 
                message_send(client.server, "KEY Q");
                break;
            case CTRL('D'): // EOF Key
                message_send(client.server, "KEY Q");
                break;
            default: // keeps going
                break;
        }
    } 
    // if client is player
    else {
        switch(c) {
        case 'h': // left
        case 'H':  
        case 'l': // right
        case 'L':  
        case 'j': // down
        case 'J':  
        case 'k': // up
        case 'K':  
        case 'y': // left diagonal up
        case 'Y':  
        case 'u': // right diagnoal up
        case 'U':  
        case 'b': // left diagonal down
        case 'B':  
        case 'n': // right diagonal down
        case 'N':  
        case 'Q': // quits
            move(1, 0);
            clrtoeol();
            sprintf(message, "KEY %c", c);
            message_send(client.server, message);
            break;
        case CTRL('D'): // EOF Key
            move(1, 0);
            clrtoeol();
            message_send(client.server, "KEY Q");
            break;
        default:
            // ignores unknown keys
            mvprintw(1, 0, "Unknown key.");
            log_e("Unknown key.");
            break;
        }

    }

    // keep looping
    return false;

}

/* ********* handleMessage ********* */
/* see client.h for description */
static bool 
handleMessage (void* arg, const addr_t from, const char* message) {

    log_v("Handling messages\n");

    // message is equal to QUIT
    if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {

        // move the pointer to after QUIT 
        const char* explanation = message + strlen("QUIT" );

        log_v("Client quits");

        // end curses
        endwin();

        // print out explanation to screen after ending window
        printf("%s\n", explanation);

        // terminate loop
        return true;
    }

    // message is equal to OK
    // at the beginning of the game
    else if (strncmp(message, "OK ", strlen("OK ")) == 0) {

        // move pointer to character after OK
        const char* letter = message + strlen("OK ");

        // update client letter
        client.letter = (*letter);

        // for testing with miniserver.c
        log_c("Player Letter: %c\n", client.letter);
    }

    // message is equal to GRID
    else if (strncmp(message, "GRID ", strlen("GRID ")) == 0) {

        // create integer to store height and width of grid
        int g_height;
        int g_width;

        log_v("Creating the grid");

        // move pointer to character after GRID
        // check if both integers are present
        if (sscanf(message + strlen("GRID "), "%d %d", &g_height, &g_width) == 2) {
            client.G_NROWS = g_height; // update game
            client.G_NCOLS = g_width;
            initializeCurses(); // make window and recheck size again
            log_v("Created the window with right grid size\n");
        } else {
            log_e("Error in handleMessage - cannot create grid\n");
        }
    }

    // message is equal to GOLD
    else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
        // move pointer to character after GOLD
        // set gold values to appropiate variables

        move(0, 0);

        // displaying status line
        // client is a spectator
        if (client.isSpec) {
            sscanf(message + strlen("GOLD "), "%d", &client.remain);
            move(0, 0);
            clrtoeol();
            log_v("Spectator checking gold\n");
            mvprintw(0, 0, "Spectator: %d nuggets unclaimed. Play at %s %s.", client.remain, client.hostname, client.port);
            refresh();
        }
        // client is a player
        else {
            if ((sscanf(message + strlen("GOLD "), "%d %d %d", &client.now_collect, &client.purse, &client.remain)) == 3) { // gold is collected
                log_v("Player collected gold\n");
                move(0, 0);
                clrtoeol();
                mvprintw(0, 0, "Player %c has %d nuggets (%d nuggets unclaimed). GOLD received: %d.", client.letter, client.purse, client.remain, client.now_collect);
                refresh();
            }
            else if ((sscanf(message + strlen("GOLD "), "%d", &client.remain)) == 1) { // gold is not collected
                log_v("Player finding gold\n");
                move(0, 0);
                clrtoeol();
                mvprintw(0, 0, "Player %c has %d nuggets (%d nuggets unclaimed). ANOTHER player collected gold!", client.letter, client.purse, client.remain);
                refresh();
            }
        }


    }

    // message is equal to DISPLAY\n
    else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {

        // move pointer to character after DISPLAY\n
        const char* map = message + strlen("DISPLAY\n");

        log_v("Displaying the map");

        // display of map
        int row = 1;
        int col = 0;
        move(row, col);

        int at_row = -1;
        int at_col = -1;

        // while not end of string
        while (*map != '\0') {
            if (*map == '$') { // it is a dollar sign, which means next line
                row++;
                col = 0;
                move(row, col);
                clrtoeol();
            } 
            else {
                if (*map == '@') {
                    at_row = row;
                    at_col = col;
                }
                printw("%c", *map);
                col++;
            }
            map++;
        }
        refresh();

        // move the cursor to the @ sign
        if ((at_row != -1) && (at_col != -1)) {
            move(at_row, at_col);
            refresh();
        }

    }
    
    // message is equal to ERROR
    // to indicate it was unable to understand or handle the client's prior message. 
    // The remainder of the line provides a short explanatory text. 
    // The client shall present this text to its user on the display's status line.
    else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {

        // move pointer to explanation
        const char* error = message + strlen("ERROR ");

        // print the error message in status line
        mvprintw(0, 55, "%s", error);
        refresh();

        // log the error
        log_e(error);
    }

    // continue looping
    return false;

}
