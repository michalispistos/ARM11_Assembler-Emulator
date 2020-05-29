CC     = gcc
CFLAGS = -Wall -g -std=c99 -pedantic -Werror -D_DEFAULT_SOURCE -Iemulate_utils
LDLIBS = -Lemulate_utils -lpipeline
BUILD  = libs emulate

all: 
	make clean
	make $(BUILD)

.SUFFIXES: .c .o

.PHONY: all clean

clean:
	/bin/rm -f $(BUILD) *.o clone
	cd emulate_utils; make clean

libs: 	
	cd emulate_utils; make

emualte: emulate.o

#assemble: assemble.o



