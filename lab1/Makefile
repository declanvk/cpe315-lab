#    Declan Kelly
#    CPE 357-09

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAINS = sizes ordering unsadd carry

all : $(MAINS)

sizes : sizes.c
	$(CC) $(CFLAGS) -o $@ $^

ordering : ordering.c
	$(CC) $(CFLAGS) -o $@ $^

unsadd : unsadd.c
	$(CC) $(CFLAGS) -o $@ $^

carry : carry.c
	$(CC) $(CFLAGS) -o $@ $^

clean : 
	rm -f $(MAINS)
