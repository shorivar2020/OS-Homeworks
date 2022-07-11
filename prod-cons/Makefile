CC = gcc
CFLAGS = -Wall -g -O2
EXTRA_CFLAGS ?= -fsanitize=address -fno-omit-frame-pointer

%.o: %.c
	$(CC) -c -o $@ $< -pthread $(CFLAGS) $(EXTRA_CFLAGS)

all: prod-cons

prod-cons: prod-cons.o
	$(CC) -o prod-cons prod-cons.c -pthread $(CFLAGS) $(EXTRA_CFLAGS)

done:
	rm -f *.[ch] Makefile