#    Declan Kelly
#    CPE 357-09

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAINS = sizes ordering unsadd sadd_over ext_field carry
MAIN = lab1
DEPS = sizes.h ordering.h unsadd.h sadd_over.h ext_field.h carry.h
OBJ = lab1.o sizes.o ordering.o unsadd.o sadd_over.o ext_field.o carry.o


all : $(MAIN)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(MAIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean : 
	rm -f *.o $(MAIN)
