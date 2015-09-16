NAME=cronmoji

CC=clang
CFLAGS=-Wall -g

all: clean cronmoji

exifcool:
	$(CC) -o $(NAME) src/$(NAME).c

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
