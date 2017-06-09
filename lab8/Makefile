#    Declan Kelly
#    CPE 357-09

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAIN = matmul2
DEPS = matmul2.c

all : $(MAIN)

$(MAIN): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(DEPS)

clean : 
	rm -f $(MAIN) *.zip

debug : CFLAGS += -g
debug : clean $(MAIN)

package : $(MAIN) $(DEPS)
	zip -u $(MAIN)_source.zip Makefile *.c *.h
