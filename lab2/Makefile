#    Declan Kelly
#    CPE 357-09

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAIN = lab2
DEPS = float_add.h float_sub.h norm_float.h unpack_float.h float_mult.h lab2.h pack_float.h unsigned_mult.h
OBJ = float_add.o float_sub.o norm_float.o unpack_float.o float_mult.o lab2.o pack_float.o unsigned_mult.o

all : $(MAIN)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(MAIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean : 
	rm -f *.o $(MAIN) *.zip

package : $(MAIN) $(DEPS)
	zip -u $(MAIN)_source.zip Makefile *.c *.h
