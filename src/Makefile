CC      = gcc
CFLAGS  = -Wall -g -std=c99 -pedantic -Werror -D_DEFAULT_SOURCE -Ilib
LDLIBS = -Llib -lemulate
BUILD  = libs emulate

all:
	make clean
	make $(BUILD)

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


