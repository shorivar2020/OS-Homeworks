CC = gcc
CFLAGS  = -Wall -pedantic
EXTRA_CFLAGS=

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(EXTRA_CFLAGS)

all: forkpipe nsd

forkpipe: forkpipe.o
	$(CC) -o forkpipe forkpipe.c $(CFLAGS)

nsd: nd.o nd.h nsd.o nsd.h nsd_main.o
	$(CC) -o nsd nd.o nsd.o nsd_main.o $(CFLAGS)

clean:
	rm -rf *.o forkpipe nsd osy04.tgz

pack:
	tar czf osy04.tgz Makefile *.[ch]

done:
	tar czf osy04.tgz Makefile *.[ch]
	rm -f *.[ch] Makefile