CC = gcc
CFLAGS = -Wall -Wextra -O3

all:
	$(CC) $(CFLAGS) src/*.c -o asciicaesar

clean:
	rm asciicaesar