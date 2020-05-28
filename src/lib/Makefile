CC = gcc
CFLAGS = -Wall -g -std=c99 -Werror -pedantic -D_DEFAULT_SOURCE
LIB =  libemulate.a
LIBOBJS = decode.o execute.o initialise.o fetch.o
BUILD = $(LIB)


all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o clone

$(LIB): 	$(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

decode.o: decode.h

execute.o: execute.h

fetch.o: fetch.h

initialise.o: initialise.h