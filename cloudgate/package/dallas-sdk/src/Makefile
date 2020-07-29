# Option Cortex specific library
LIB_OWCORTEX_FILENAME = libowirecortex.so

LIB_OWCORTEX_SOURCE_PATH=lib/general/link/CORTEX

OBJS_OWCORTEX_LIBDEVICE= $(LIB_OWCORTEX_SOURCE_PATH)/cortexll.o $(LIB_OWCORTEX_SOURCE_PATH)/cortexses.o

#all: $(LIB_OWCORTEX_FILENAME)
owire_lib: $(LIB_OWCORTEX_FILENAME)

$(LIB_OWCORTEX_FILENAME): $(OBJS_OWCORTEX_LIBDEVICE)
	$(CC) -shared -fPIC -o $@ $^

DEBUG=-ggdb

OWIRE_LDFLAGS = $(LDFLAGS)

%.o: %.c
	$(CC) -Iinclude -I. -Icommon/  $(CFLAGS) $(DEBUG) -Wall -fPIC -shared -c $< -o $@

include makefile_cortex.mk
