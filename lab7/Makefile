#    Declan Kelly
#    CPE 357-09

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAIN = lab7
DEPS = mips_asm_header.h mips_context.h common.h
OBJ = load_instructions.o execute_instructions.o summarise_context.o cpu_utils.o lab7.o mips_pipes.o

all : $(MAIN)

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(MAIN): $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean : 
	rm -f $(MAIN) *.zip *.o

debug : CFLAGS += -g
debug : clean $(MAIN)

package : $(MAIN) $(DEPS)
	zip -u $(MAIN)_source.zip Makefile *.c *.h
