# CS50 Nuggets
## Design Spec
### Bowling, Fall, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes game, player, serverDisplay and serverMove modules.
We describe the client module then each of the server modules together.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Client

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

See the requirements spec for both the command-line and interactive UI.

The *client* module allows joining the game as a player or spectator, depending on whether *playername* is provided.
```bash
./client hostname port [playername]
```
As a player, the user can press all the keys, and non-allowed keys will result in an error message on the status line.
As a spectator, the user can only press on key Q or CTRL D (EOF). Other keys will not register, and no messages will be sent to the server.

Client logging:

```bash
./client 2>player.log hostname port playername
```
^logs relevant information to the "player.log" file when joining as a player.


```bash
./client 2>spectator.log hostname port
```
^logs pertinent information to the "spectator.log" file when joining as a spectator.

### Inputs and outputs

#### Input:
##### Received from user
- PLAYER: Q/EOF, h, H, l, L, j, J, y, Y, u, U, b, B, n, N
- SPECTATOR: Q/EOF

#### Output:
##### Messages to server:
- PLAY: if joining as a player
- SPECTATE: if joining as a spectator
- the keys
##### Display to user:
- grid
- status line
- game information

The client module also logs information to a file.

### Functional decomposition into modules

We anticipate the following module:
1. *client* module

We anticipate the following methods within the *client* module:
1. *main*, which opens file to log information, parses command-line arguments, sends a message to the server, initializes ncurses and constantly listens for messages.
2. *initializeCurses*, which initializes the ncurses library and calls *screenSize*. 
3. *screenSize*, which prompts the user to resize their window until it meets the requirements.
4. *handleInput*, which handles inputs from the user (keystrokes).
5. *handleMessage*, which handles messages from the server.

### Pseudo code for logic/algorithmic flow

The client will run as follows:

- opens the file for logging information
- initializes message module
- parses command-line arguments
- sets address of the server
- sends message to server about which type of client it is (player/spectator)
- initializes ncurses and checks for screen size 
- constantly listens for messages from server and user (more explanation in client README.md)
- ends message module
- close file for logging

We describe each of the major functions within the “functional decomposition into modules” section.

### Major data structures

There is one data structure called the `client` structure. It stores the following data:
- letter of the player (received after "OK k" from the server, where k is a letter)
- purse, which is how much gold the player has
- now_collect of the player, which is how much gold the player just collected
- remain, which is how much gold still remains in the game
- isSpec, which is if the client is a spectator or not
- server, which is the address of the server
- hostname, which is the hostname of the server
- port, which is the port of the server
- G_NROWS, which is the number of rows of the grid sent from the server
- G_NCOLS, which is the number of columns of the grid sent from the server

---

## Server
### User interface

See the requirements spec for the command-line interface.
There is no interaction with the user.

### Inputs and outputs

Input:
Messages from clients: PLAY, SPECTATE, KEY

Output:
Messages to clients: OK, GRID, GOLD, DISPLAY, QUIT, ERROR
Log useful information to stderr
Write to log files (as described in user interface section)

### Functional decomposition into modules

Other than the main *server* module, we also anticipate the following modules:
	1. *game*, which 
	2. *player* , which 
	3. *serverDisplay*, which contains most of the functions that handle the messages received by and to the client
	4. *serverMove*, which contains the functions that occur when a player moves
	

We anticipate the following functions within each module:
Within the *server* module, we anticipate:
 - `int main(int argc, char *argv[])`
   - Main function to start the server.
- `bool initializeGame(char* map)`
   - Initializes the game structure, validates the map, and sets up the game environment.
- `bool validateMap(char* map, int numRows, int numCols)`
   - Validates the provided map by checking if it has a .txt extension and ensuring it's a square map.
- `bool endsWithTxt(char *str)`
   - Checks if a string ends with ".txt".
- `bool loadMap(char* map)`
   - Loads the map from the provided map file and updates the game structure.
- `static bool handleMessage(void* arg, const addr_t from, const char* message)`
   - Handles incoming messages from clients, including PLAY, SPECTATE, and KEY messages.

Within the *game* module, we anticipate the following functions:
- `game_new(int numRows, int numCols, int maxPlayers)`
  - Creates a new game structure with the specified number of rows, columns, and maximum players.
  - Initializes the map, gold locations, and other game-related data.
- `game_delete(game_t *g)`
  - Deletes the game structure and frees the associated memory.
- `game_fillMapFromFile(game_t *game, const char *mapFilename)`
  - Reads the map data from a specified map file and fills the game's map array.
- `game_initializePlayersStartLocations(game_t* game, int seed)`
  - Initializes the start positions for players in the game based on a seed value.
- `game_initializeGold(game_t *game, int seed, int GoldTotal, int GoldMinNumPiles, int GoldMaxNumPiles)`
  - Initializes the gold piles in the game based on a seed value and specified parameters.
-`game_getNextLetter(game_t *game)`
  - Retrieves the next available player character letter and updates the game's letter index.
- `game_addPlayer(game_t *game, const char* playerName, addr_t address)`
  - Adds a new player to the game with the given player name and address.
- `game_addSpectator(game_t* game, addr_t* address)`
  - Adds a spectator to the game with the given address.
- `game_removeSpectator(game_t* game)`
  - Removes the spectator from the game.
- `game_sendScoreTable(game_t* game)`
  - Sends a score table message containing player information to all players in the game.

Within the  *serverDisplay* module, we anticipate:
- `isInt`: Checks if a value is an integer.
- `isRoomSpot`: Checks if a point (x, y) is within a room spot.
- `addVisiblePoints`: Adds visible points to the player's visible points array.
- `isVisible`: Checks if a point (px, py) is visible from another point (cx, cy).
- `display`: Displays the game map for a player.
- `addPlayerToMap`: Adds a player and their character to the map.
- `mapToOneArray`: Converts a map from a 2D array to a one-dimensional string.


Within the  *serverMove*, module, we anticipate:
- `requestMove(addr_t* address, char key)`
  - Request a player's move and update the game state accordingly.
- `get_target_location(game_t *game, player_t *player, char key)`
  - Get the target location based on the player's move key.
- `quitPlayer(player_t *player)`
  - Handle a player's quit request.
- `isWall(game_t *game, point_t *target)`
  - Check if a target location is a wall.
- `alreadyPlayer(point_t *target)`
  - Check if there is already a player at the specified target location.
- `swapSpots(player_t *player1, point_t *target)`
  - Swap spots between two players.
- `isPlayerHere(void *arg, const addr_t from, const char* message)`
  - Callback function to check if a player is at the specified location.
- `whatPlayerHere(void *arg, const char *key, void *item)`
  - Find a player at a given location.
- `isGold(int** goldLocations, int numRows, int numCols, point_t* target)`
  - Check if a target location has gold.
- `changeGold(player_t* player, int amountGold)`
  - Change the gold amount at a location and update the player's gold.
- `updatePlayerLocation(player_t* player, point_t* target)`
  - Update a player's location.
- `nuggetsLeft()`
  - Calculate the total number of gold nuggets left in the game.
- `sendGoldMessage(int n, int p, int r)`
  - Send a GOLD message to a player or spectator.


Within the *PLAYER* module, we anticipate:
- `player_new(game_t* game, const char* name, const addr_t address)`
  - Create a new player structure with the specified game, name, and address.
- `player_delete(player_t* player, const game_t* game)`
  - Delete a player structure and free the associated memory.


### Pseudo code for logic/algorithmic flow

The server will run as follows:

# Game Initialization
- Parse command-line arguments to determine the number of rows, columns, and maximum players.
- Initialize the game structure using `game_new`.
- Load the map from a map file using `loadMap`.
- Validate the map to ensure it has a .txt extension and is a square map using `validateMap`.

# Player and Spectator Management
- Handle incoming client connections.
- When a new player connects:
   - Create a new player structure using `player_new`.
   - Add the player to the game using `game_addPlayer`.
   - Assign a player letter and initialize their properties.
   - Update the game state.
- When a new spectator connects:
   - Add the spectator to the game using `game_addSpectator`.
   - Update the game state.

# Main Game Loop
- Start the main game loop:
   - Listen for incoming messages from clients using `handleMessage`.
   - Process PLAY, SPECTATE, and KEY messages.
   - Update player positions, handle player movements, and interactions using `requestMove`.
   - Check for player quits using `quitPlayer`.
   - Check for gold pickups and update player wallets.
   - Update visible points for players using `serverDisplay`.
   - Broadcast game status and score tables to players and spectators using `game_sendScoreTable`.
   - Handle any other game-specific logic.
   
# Game Cleanup
- When the game ends:
   - Clean up player and spectator structures using `player_delete` and `game_removeSpectator`.
   - Delete the game structure using `game_delete`.
   

### Major data structures
We have two main data structures and the extensive description is in the Implementation spec.

### `game_t` Structure; 
A game structure with the specified number of rows, columns, and maximum players.
  - It also Initializes the map, gold locations, and other game-related data.

### `player_t` Structure:
   - The `player_t` structure represents a player in a game. It stores essential player-related data, including the player's name, a unique letter for identification on the game map, current location, visibility information, collected gold (wallet), game participation status, and the player's Internet address.


### Testing

### Unit Testing

We expect to have unit testers for each tester modules, namely one for the map structure, one for the entire_map/game structure, one for the server and one for the client.

We expect to develop the server/client modules independently of the game module, to ensure that the server and clients are correctly functioning and able to send and receive messages (specified by message.h) between both before implementing the game onto the server.

We plan to first implement a simplified version of the game without visibility before implementing the full game.

To test, we first made sure the client side works by testing through logging and miniserver.c. After that, we tested initializing the server without display or move. Then, we checked without move -- just with display. Lastly, we implemented the whole game and checked our progress through the log files and print statements. 

### Integration Testing

Once each of the modules functions correctly independently, we can then test the game as a whole. Important edge cases to consider include:
Incorrect number of arguments/incorrect initialization of the server and/or client
Switching spectators
If too many players try to join
If a player unexpectedly quits the game/If the server unexpectedly crashes
If the connection between the server and client is broken
If an invalid message is sent between the server and client
