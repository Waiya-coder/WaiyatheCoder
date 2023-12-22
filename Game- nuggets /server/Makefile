# Makefile for 'support' folder
#
# Memo Tangtipongkul, Fall 2023

OBJS = server.o game.o player.o serverDisplay.o serverMove.o
LIBS = ../support/support.a ../libcs50/libcs50-given.a

#uncomment the following to turn on verbose memory logging
TESTING=-DMEMTEST 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc
MAKE = make

server: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm -lncurses

server.o: server.h ../support/message.h game.o
game.o: game.h player.o ../libcs50/set.h
player.o: player.h game.h ../support/message.h
serverDisplay.o: serverDisplay.h server.h ../libcs50/set.h
serverMove.o: serverMove.h server.h ../support/message.h ../libcs50/set.h

.PHONY: test valgrind clean

# assuming `test.pairs` exists

# test: hashtabletest
# 	./hashtabletest

# valgrind: hashtabletest
# 	valgrind ./hashtabletest

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f server