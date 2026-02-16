# Name: Brenda Waiya
# Class: CS50 Fall 2023


L = ../libcs50

PROG = 
OBJS = 
LIBS =  $L/libcs50-given.a
LLIBS = ../common/common.a

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L  -I../common
CC = gcc
MAKE = make

all: indexer indextest

indexer: indexer.o  $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o  $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY:  test valgrind clean

test: indexer
	bash -v testing.sh &> testing.out
  
valgrind: all 
	valgrind ./indexer
	valgrind ./indextest
	
clean:
	rm -f *~ *.o *.dSYM
	rm -f indexer indextest
	rm -f core
	