CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic -D_DEFAULT_SOURCE -O3
LIB     = libemulate_utils.a
LIBOBJS = pipeline.o pipeline_utils.o initialise.o
BUILD   = $(LIB)


all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o clone

$(LIB): 	$(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

pipeline.o: pipeline.h

pipeline_utils.o: pipeline_utils.h

initialise.o: initialise.h


