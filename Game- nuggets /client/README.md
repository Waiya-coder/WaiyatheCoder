# CS50 Nuggets (Bowling) - client
## CS50 Fall 2023

### client

The `client` program communicates with the server program.
There is one structure inside, which keeps track of the status of the game and the information for 1 `client`.
The `client` can be a _player_ or a _spectator_.
The `client` displays that information sent from the server, including the display of the map, and the `client` status.

### Usage

The *client* module, defined in `client.h` and implemented in `client.c`, exports the following functions:

```c
int main (const int argc, const char* argv[]);
static void initializeCurses();
static void screenSize();
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);
```
There is one structure in the program, which is a client structure. It keeps track of the following:

```c
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
```

### Implementation

We implement client using the defined structure above.

The `main` function opens a file to write in log messages, parses the command-line input, making sure that the format is correct. Then, it determines whether the client is a _player_ or a _spectator_ by accessing the number of command-line arguments. After that, it sends a message to the server. The main function calso calls the `initializeCurses` method, which creates a window display and prompts the user to change their window size to the correct dimensions. Lastly, it calls `message_loop()` which takes `handleInput` and `handleMessage` as arguments.

The `initializeCurses` method initializes the window screen and calls `screenSize` to check if the window is big enough for the display.

The `screenSize` method takes in the grid size from the server, and makes sure that the client's window is atleast NR+1 by NC+1.

The `handleInput` method takes input from the user, which are key inputs. It sends message "KEY k" to the server, where 'k' is the key that the user pressed.

The `handleMessage` method takes messages from the user. It is assumed that the user will send a correctly formatted string. It parses the string, and compares the first word to eah of the following cases:
- QUIT: the window ends, and the quit message is displayed for the user
- OK: we update the client's letter to the letter that the user sent
- GRID: we update the client.G_NROWS and client.G_NCOLS, then calls `initializeCurses` again
- GOLD: we update the client's gold information (purse, now_collect, remain)
- DISPLAY\n: we display the status line and also the map
- ERROR: we display the error message on the status line, and log the error message

### Assumptions

No assumptions beyond those that are clear from the spec.

The string message from the server is formatted correctly.

The errors from valgrind are from the ncurses library.

### Files

* `Makefile` - compilation procedure
* `client.h` - the interface
* `client.c` - the implementation
* `miniserver.c` - small server to test client program
* `logclientout` - a file that logs the client program

### Compilation

To compile, simply `make all`.

### Testing

The `miniserver.c` is taken from Professor Kotz's miniserver.c. We have modified it to be able to handle messages from the client.c.

As a spectator, the user will see a static display line and a map.
As a player, the user can press the key "h" to "collect gold" and see a change in the map and status. The use can also press the key "n" to simulate when an error occurs.
For both types of clients, the user can press "Q" or "CTRL D (EOF)" to quit.

To run the test server, simply `make miniservertest`. Then run the client and connect to the correct port.
See `logclientout` for details of testing.

To test with valgrind, run the client with valgrind --leak-check=full --show-leak-kinds=all.
