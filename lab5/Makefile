#    Declan Kelly
#    CPE 357-09

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAIN = lab5
DEPS = mips_asm_header.h
OBJ = load_instructions.o

all : $(MAIN)

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(MAIN): $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean : 
	rm -f $(MAIN) *.zip

package : $(MAIN) $(DEPS)
	zip -u $(MAIN)_source.zip Makefile *.c *.h
