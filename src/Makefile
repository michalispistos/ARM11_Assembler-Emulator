CC      = gcc
CFLAGS  = -Wall -g -std=c99 -D_POSIX_SOURCE -D_BSD_SOURCE -Ilib
#-pedantic-Werror 
LDLIBS = -Llib -lemulate
BUILD = libs emulate

all: $(BUILD)

.SUFFIXES: .c .o

.PHONY: all clean

clean:
	/bin/rm -f $(BUILD) *.o clone
	cd lib; make clean
	#rm -f $(wildcard *.o)
	#rm -f assemble
	#rm -f emulate

libs: 	
	cd lib; make

emualte: emulate.o


#assemble: assemble.o

#emulate: emulate.o
#	$(CC) emulate.c lib/initialise.c lib/fetch.c lib/decode.c lib/execute.c -o emulate


