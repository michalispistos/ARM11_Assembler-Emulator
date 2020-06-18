CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic -D_DEFAULT_SOURCE -O3 -I..
LIB     = libassemble_utils.a
LIBOBJS = assemble_instr.o map.o parser.o two_pass.o assemble_instr_utils.o ../common_utils.o
BUILD   = $(LIB)


all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o clone

$(LIB): 	$(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)
	
assemble_instr.o: assemble_instr.h

map.o: map.h

parser.o: parser.h

two_pass.o: two_pass.h

assemble_instr_utils.o: assemble_instr_utils.h

