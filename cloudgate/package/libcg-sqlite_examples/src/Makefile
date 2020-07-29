all: examples

OBJS  = $(patsubst %.c,%.o,$(wildcard *.c))
BINS  = $(patsubst %.o,%,$(OBJS))

DEBUG = -ggdb #-O0
override LDFLAGS += -lcg -lpthread -ljson-c -lsqlite3

examples: $(BINS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -fPIC -Wall -Werror -I. -std=gnu99 -c $(<) -o $(@)

$(BINS): %: %.o
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean examples
