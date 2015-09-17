NAME=cronmoji

CC=clang
CFLAGS=-Wall -g
INCLUDES=pcg_basic

all: clean cronmoji

cronmoji:
	$(CC) -o $(NAME) $(NAME).c $(INCLUDES).c

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
