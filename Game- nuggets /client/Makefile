# Makefile for the "client" program.
#
# Memo Tangtipongkul, Fall 2023

OBJS = client.o
OBJS2 = miniserver.o
LIBS = ../support/support.a

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

client: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

miniserver: $(OBJS2) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

client.o: client.c ../support/message.h ../support/log.h

miniserver.o: miniserver.c ../support/message.h ../support/log.h

.PHONY: all test clean

all: client miniserver

miniservertest: miniserver
	./miniserver

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f client
	rm -f miniserver
	rm -f core
	rm -f logclientout.txt