NAME = cronmoji

CC = clang
CFLAGS = -Wall -g

SRCS = $(wildcard src/*.c)

all: clean cronmoji

cronmoji:
	$(CC) -lcurl -o $(NAME) $(SRCS)

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
