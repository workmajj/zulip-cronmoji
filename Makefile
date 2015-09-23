NAME = cronmoji

CC = clang
CFLAGS = -Wall -g

all: clean cronmoji

cronmoji:
	$(CC) -o $(NAME) -lcurl src/$(NAME).c

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
