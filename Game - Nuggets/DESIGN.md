# CS50 Nuggets
## Design Spec
### Bowling, Fall, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes client_input, client_send, server_inout, server_move, and server_init modules.
We describe each of the client modules together and each of the server modules together.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

See the requirements spec for both the command-line and interactive UI.

The *client* module allows joining the game as a player or spectator, depending on whether *playername* is provided.
```bash
./client hostname port [playername]
```

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

Input:
User keystrokes (PLAYER: Q, h, l, j, y, u, b, n; SPECTATOR: Q)

Output:
Messages to server:
- PLAY: if joining as a player
- SPECTATE: if joining as a spectator
- the keys
Display to user:
- grid
- status line
- game information
Log information to stderr
Write to log files (as described in user interface section)

### Functional decomposition into modules

We anticipate the following modules:

Other than the main *client* module, we also anticipate the following modules:
	1. *client_input*, which contains the functions that handle the messages received by the client
	2. *client_send*, which contains the functions that sends messages to the server

We anticipate the following functions within each module:
Within the *client* module, we anticipate:
	1. *main*, which parses command-line arguments, initializes modules and constantly listens for messages
  2. *connect*, which connects to the server. 
	3. *displayMap*, which displays the map, including the gold and players

Within the *client_send* module, we anticipate:
	1. *whoClient*, sends to the server whether it’s a participant or server
	2. *mapSize*, which sends the server the map size
	3. *makeMove*, which sends the server a message when the player moves or quits

Within the *client_input* module, we anticipate:
	1. *receiveInput*, which receives input from the server and calls the function that handles that kind of message
	2. *input_quit*, which quits and exits the client if told to quit from the server (ex. if game ends)
	3. *input_gold*, which deals with changes in gold
	4. *input_player*, which deals with changes in positions of other players
	5. *input_map*, which receives information about changes in your map, whether it’s because you move or others move

 
### Pseudo code for logic/algorithmic flow

The client will run as follows:
	parses command-line arguments
	connects to server
	is constantly listening for messages from server
	checks if it’s a participant or server
	^sends to server
	sends server the map size
	^if server says not big enough, pause until it is big enough
	receives map from server, including gold and other players
	displays map
	if it’s a player, check if player makes moves
		if it’s a valid key (for moving or quitting)
			send move to server
	if server tells you something changes, update and display new map or quit
	clean up

We describe each of the major functions within the “functional decomposition into modules” section.

### Major data structures

Helper modules provide data structures we use:
- *hashtable*s are used in a variety of situations, including:
- where the players are
		key: player, item: array of location (x,y)
- where the gold is
		key: x value where key exists, item: array (y value, how much gold is worth)
- who has gold
		key: player name, item: how much gold they have

We also create two other main data structures.
- the *game* struct includes the following elements:
- the entire map
		an array of strings
- where the players are
hashtable, key: player, item: array of location (x,y)
- where the gold is
	hashtable, key: x value where key exists, item: array (y value, how much the gold is worth)
- who has gold
	hashtable, key: player name, int: how much gold
- where players start
This game struct is used within the server to keep track of the current map and other information. This information is also sent to the spectator, who receives all this information. This information is also used to decide what the players can see and their maps.
- the *player_map* struct includes the following elements:
  - the map that the player can see
		array of strings
	- where the players are that it can see
		hashtable, key: player, item: array of location (x,y)
- where the gold is that it can see
		hashtable, key: x value where key exists, item: array (y value, how much the gold is worth)
  - number of nuggets the player has (starts at 0)

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
	1. *server_inout*, which contains most of the functions that handle the messages received by and to the client
	2. *server_move*, which contains the functions that occur when a player moves
	3. *server_init*, which contains the functions that initialize the server

We anticipate the following functions within each module:
Within the *server* module, we anticipate:
	1. *main*, which parses command-line arguments, initializes modules, and constantly listens to see if client is connecting or sends a message.

Within the *server_init* module, we anticipate:
	1. *initializeGame*, which initializes the game struct and then calls the other functions that initialize the server
	2. *validateMap*, which checks if it is a valid map
	3. *load_map*, which takes in map.txt and represents the map as an array of strings
	4. *initialize_gold*, which uses the seed to add random numbers between GoldMinNumPiles and GoldMaxNumPiles to an array until the total amount reaches GoldTotal. It then puts gold in random spots (validating if it’s a place gold can be) and saves them to the hashtable within the game struct
	5. *isValidGoldSpot*, which checks to see if a spot is a place where gold can be
	6. *wherePlayersStart*, which uses the seed to pick a spot where all players start and adds the value to the struct

Within the *server_inout* module, we anticipate:
	1. *receiveInput*, which receives input from the client and calls the function that handles that kind of message
	2. *addPlayer*, which adds a new player to the game, both internally and externally
	3. *addSpec*, which adds a new spectator to the game, both internally and externally
	4.*checkSpec*, which checks if there is already one spectator
	5.*quitPlayer*, which tells the player to quit. It also removes this player from the game struct and each player’s player_map
	6.*isMapGood*, which checks if the player’s screen is large enough for the map, send message back down
	7. *sendMap*, which sends the new map down to client

Within the *server_move* module, we anticipate:
	1. *isWall*, which checks if they move into a wall or somewhere they can’t go; if so, don’t move
	2.*alreadyPlayer*, which chicks if there’s another player there and calls *swapSpots* if so.
	3. *swapSpots*, which swaps the players’ spots, change entire_map’s player ht to players’ new locations, change each player_map’s player ht to players’ new locations
	4.*isGold*, which checks if there’s gold in that spot. If so, remove it from the gold ht in game struct and add it to who-has-gold ht. Remove that gold from players’ visibility (if they could see it). Add that amount of gold to that player’s struct.
	5.*isMoreGold*, checks if all gold has been collected. if so, end game by sending a QUIT message to all clients with a summary of the game
	6. *changeGameMap*, which update the game struct to have player’s new location
	7. *changePlayersMap*, which changes each player_map to player’s new location (if within player’s visibility)
	8. *whereVisible*, which calculates, based on where the player is, what is visible to them

### Pseudo code for logic/algorithmic flow

The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	constantly listens to see if client is connecting or sends a message
initializes the game struct and then calls the other functions that initialize the server
checks if it is a valid map
takes in map.txt and represents the map as an array of strings
	 uses the seed to add random numbers between GoldMinNumPiles and GoldMaxNumPiles to an array until the total amount reaches GoldTotal. It then puts gold in random spots (validating if it’s a place gold can be) and saves them to the hashtable within the game struct
	uses the seed to pick a spot where all players start and adds the value to the struct
	receives input from the client and calls the function that handles that kind of message
	if new player
adds a new player to the game, both internally and externally
	if new spectator
		checks if there is already one spectator
adds a new spectator to the game, both internally and externally
checks if map is large enough, send message back down
send new map down to client
if player moves
	checks if they move into a wall or somewhere they can’t go; if so, don’t move
	checks if there’s another player there and calls *swapSpots* if so.
	swaps the players’ spots, change entire_map’s player ht to players’ new locations, change each player_map’s player ht to players’ new locations
	checks if there’s gold in that spot. If so, remove it from the gold ht in game struct and add it to who-has-gold ht. Remove that gold from players’ visibility (if they could see it). Add that amount of gold to that player’s struct.
	checks if all gold has been collected. if so, end game by sending a QUIT message to all clients with a summary of the game
	update the game struct to have player’s new location
	changes each player_map to player’s new location (if within player’s visibility)

We describe each of the major functions within the “functional decomposition into modules” section.

### Major data structures

We also create our main data structure.
-the *player_map* struct includes the following elements:
	-the map that the player can see
		array of strings
	-where the players are that it can see
		hashtable, key: player, item: array of location (x,y)
	-where the gold is that it can see
		hashtable, key: x value where key exists, item: array (y value, how much the gold is worth)
	-number of nuggets the player has (starts at 0)

Helper modules provide data structures we use:
	-*hashtable*s are used in a variety of situations, including within our player_map struct, as we just described

> Describe each major data structure in this program: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
> Mention, but do not describe, data structures implemented by other modules (such as the new modules you detail below, or any libcs50 data structures you plan to use).

###Testing

### Unit Testing

We expect to have unit testers for each tester modules, namely one for the map structure, one for the entire_map/game structure, one for the server and one for the client.

 We expect to develop the server/client modules independently of the game module, to ensure that the server and clients are correctly functioning and able to send and receive messages (specified by message.h) between both before implementing the game onto the server.

We plan to first implement a simplified version of the game without visibility before implementing the full game.

### Integration Testing

Once each of the modules functions correctly independently, we can then test the game as a whole. Important edge cases to consider include:
Incorrect number of arguments/incorrect initialization of the server and/or client
Switching spectators
If too many players try to join
If a player unexpectedly quits the game/If the server unexpectedly crashes
If the connection between the server and client is broken
If an invalid message is sent between the server and client
