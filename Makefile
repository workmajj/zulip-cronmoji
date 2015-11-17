NAME = cronmoji

CFLAGS = -Wall -g -std=c99
LDFLAGS = -lcurl

OBJS = src/cronmoji.o

cronmoji: $(OBJS)
	$(CC) $(LDFLAGS) -o cronmoji $(OBJS)

.PHONY: clean
clean:
	rm -f $(NAME)
	rm -rf $(NAME).dSYM
	rm -f $(OBJS)
