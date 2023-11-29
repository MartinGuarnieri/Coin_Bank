CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS =


all:	scale_test

###

display_test:	scale_test.o
	$(CC) $(LFLAGS) -o scale_test scale_test.o

###

clean:	
	rm -f *~ *.o scale_test

