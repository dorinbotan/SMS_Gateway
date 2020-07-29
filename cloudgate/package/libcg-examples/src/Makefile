all: examples

OBJS  = $(patsubst %.c,%.o,$(wildcard *.c))
BINS  = $(patsubst %.o,%,$(OBJS))

DEBUG = -ggdb #-O0
override LDFLAGS += -lsms -lcg -lpthread -ljson-c

examples: $(BINS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -fPIC -Wall -Werror -I. -std=gnu99 -c $(<) -o $(@)

$(BINS): %: %.o
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)

# 'Make' uses the last recipe specified for a target...
# So this one is used in stead of the more general one above
example: example.o
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean examples
