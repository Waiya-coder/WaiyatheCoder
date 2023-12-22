/*
 * miniserver - a simple server using the messaging module
 *
 * Memo Tangtipongkul - Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include "../support/message.h"
#include "../support/log.h"

/**************** file-local functions ****************/

static bool handleMessage(void* arg, const addr_t from, const char* message);

/***************** main *******************************/
int
main(const int argc, char* argv[])
{
  // initialize the message module (without logging)
  int myPort = message_init(NULL);
  if (myPort == 0) {
    return 2; // failure to initialize message module
  } else {
    printf("serverPort=%d\n", myPort);
  }

  // check arguments (there should be none)
  const char* program = argv[0];
  if (argc != 1) {
    fprintf(stderr, "usage: %s\n", program);
    return 3; // bad commandline
  }
  
  // Loop, waiting for input or for messages; provide callback functions.
  // We use the 'arg' parameter to carry a pointer to 'server'.
  bool ok = message_loop(NULL, 0, NULL, NULL, handleMessage);

  // shut down the message module
  message_done();
  
  return ok? 0 : 1; // status code depends on result of message_loop
}

/**************** handleMessage ****************/
/* Datagram received; print it, read a line from stdin, and use it as reply.
 * We ignore 'arg' here.
 * Return true if EOF on stdin or any fatal error.
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{
  if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
    message_send(from, "OK A");
    message_send(from, "GRID 20 90");
    message_send(from, "GOLD 0 0 200");
    message_send(from, "DISPLAY\n||||||||$welcome to the new line *A@$last line!\n");
  }

  else if (strncmp(message, "SPECTATE ", strlen("SPECTATE ")) == 0) {
    message_send(from, "GRID 20 90");
    message_send(from, "GOLD 0 0 200");
    message_send(from, "DISPLAY\n||||||||$welcome to the new line *A@$last line!\n");
  }

  else if (strncmp(message, "KEY Q", strlen("KEY Q")) == 0) {
    message_send(from, "QUIT client quits!");
  }

  else if (strncmp(message, "KEY h", strlen("KEY h")) == 0) {
    message_send(from, "GOLD 13 20 180");
    message_send(from, "DISPLAY\nYOU JUST$COLLECTED&SOME GOLD$ CONGRASTSSSSSSS!!!\n");
  }

  else if (strncmp(message, "KEY n", strlen("KEY n")) == 0){
    message_send(from, "ERROR wrong key");
  }

  // normal case: keep looping
  return false;

}
