#Nakefile for crawler
# Taken largely from the makefile in bag 
# Brenda Waiya
#10/15/2023
#shortcut for directory




L = ../libcs50
C = ../common
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
PROG = crawler
OBJS = crawler.o 
LIBS = $L/libcs50-given.a $C/common.a

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: test valgrind clean all
	
# executable depends on object files
# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# object files depend on header files
$(OBJS): $L/bag.h $L/file.h $L/webpage.h $L/mem.h $L/hashtable.h $C/pagedir.h

# Invokes bash script testing
test: $(PROG) testing.sh
	bash -v testing.sh &> testing.out

#  test: crawler
# 	./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2
	
valgrind: crawler test_val.sh
	$(VALGRIND) bash -v test_val.sh  &> test_val.out

all: $(PROG) testing.sh
	make $(PROG)
	bash -v testing.sh
	$(VALGRIND) bash -v testing.sh
# clean up after our compilation
clean:
	rm -f core
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -rf ../data

	
#