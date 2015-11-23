CC = clang
CFLAGS = -MD -Wall -Werror -std=c99
LDLIBS = -lcurl

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

cronmoji: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o cronmoji

.PHONY: clean
clean:
	rm -f cronmoji
	rm -f $(OBJS) $(DEPS)
