NAME = cronmoji

CC = clang
CFLAGS = -Wall -g

SRCS = $(wildcard *.c)

all: clean cronmoji

cronmoji:
	$(CC) -o $(NAME) $(SRCS)

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
