all: alibaba

SRCS = $(shell find . -name "*.c")
OBJS = $(SRCS:%=%.o)

DEBUG = -ggdb #-O0

%.c.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -I. -c $(<) -o $(@)

alibaba: $(OBJS)
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)
 
clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean alibaba
