CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic -D_DEFAULT_SOURCE
LIB     = libassemble_utils.a
LIBOBJS = assemble_instr.o map.o parser.o two_pass.o
BUILD   = $(LIB)


all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o clone

$(LIB): 	$(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

assemble_instr.o: assemble_instr.c

map.o: map.c

parser.o: parser.c

two_pass.o: two_pass.c

