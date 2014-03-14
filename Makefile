CFLAGS=-std=gnu99 -g -O3 -Wall -Wextra -Werror

all: mmxlviii

mmxlviii: game.o interface.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf mmxlviii
