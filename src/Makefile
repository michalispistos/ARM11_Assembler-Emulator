CC     = gcc
CFLAGS = -Wall -g -std=c99 -pedantic -Werror -D_DEFAULT_SOURCE -Iemulate_utils -Iassemble_utils
LDLIBS = -Lemulate_utils -Lassemble_utils -lemulate_utils -Lassemble_utils
BUILD  = libs emulate assemble 

all: 
	make clean
	make $(BUILD)

.SUFFIXES: .c .o

.PHONY: all clean

clean:
	/bin/rm -f $(BUILD) *.o clone
	cd emulate_utils; make clean
	cd assemble_utils; make clean

libs: 	
	cd emulate_utils; make
	cd assemble_utils; make

emualte: emulate.o

assemble: assemble.o



