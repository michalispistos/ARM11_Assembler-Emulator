CC      = gcc
CFLAGS  = -Wall -g -std=c99 
#-D_POSIX_SOURCE -D_BSD_SOURCE -Werror -pedantic
BUILD = emulate

.SUFFIXES: .c .o

.PHONY: all clean

all: $(BUILD)

assemble: assemble.o

emulate: emulate.o
	$(CC) emulate.c lib/initialise.c lib/fetch.c lib/decode.c lib/execute.c -o emulate

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
