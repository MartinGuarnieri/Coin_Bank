CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS =


all:	scale_test display_amount

###

scale_test:	scale_test.o
	$(CC) $(LFLAGS) -o scale_test scale_test.o

scale_test.o:	scale_test.c
	$(CC) $(LFLAGS) -c scale_test.c

###

###
display_amount:	display_amount.o write_display.o
	$(CC) $(LFLAGS) -o display_amount display_amount.o write_display.o

display_amount.o:	display_amount.c write_display.h
	$(CC) $(LFLAGS) -c display_amount.c

###

###
write_display.o:	write_display.c write_display.h
	$(CC) $(LFLAGS) -c write_display.c

###

clean:	
	rm -f *~ *.o scale_test display_amount write_display

