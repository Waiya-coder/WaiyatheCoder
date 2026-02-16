# Brenda Waiya
#10/15/2023
OBJS = pagedir.o index.o word.o 
LIB = common.a
LLIBS = ../libcs50/libcs50-given.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
Make = make


#ar - archives
common.a: $(OBJS)
	ar cr $(LIB) $(OBJS) $(LLIBS)

pagedir.o: pagedir.h ../libcs50/webpage.h ../libcs50/file.h
index.o: index.h ../libcs50/hashtable.h ../libcs50/file.h
word.o: word.h


.PHONY : clean


clean:
	rm -f *~ *.o *.a
	rm -f core
	rm -f common.a *~ *.o