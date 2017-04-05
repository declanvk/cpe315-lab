#    Declan Kelly
#    CPE 357-09

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAINS = sizes ordering unsadd sadd_over ext_field carry

all : $(MAINS)

sizes : sizes.c
	$(CC) $(CFLAGS) -o $@ $^

ordering : ordering.c
	$(CC) $(CFLAGS) -o $@ $^

unsadd : unsadd.c
	$(CC) $(CFLAGS) -o $@ $^

sadd_over : sadd_over.c
	$(CC) $(CFLAGS) -o $@ $^

ext_field : ext_field.c
	$(CC) $(CFLAGS) -o $@ $^

carry : carry.c
	$(CC) $(CFLAGS) -o $@ $^

demo : clean $(MAINS)
	./sizes && ./ordering && ./unsadd && ./carry && ./sadd_over && ./ext_field

clean : 
	rm -f $(MAINS)
