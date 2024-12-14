CC=gcc
CFLAGS=-Wall -Wextra

all: threaded rps threadedandlocked
threaded: threaded.c
	$(CC) $(CFLAGS) -o threaded threaded.c
rps: rps.c
	$(CC) $(CFLAGS) -o rps rps.c
threadedandlocked: threadedandlocked.c
	$(CC) $(CFLAGS) -o threadedandlocked threadedandlocked.c

clean:
	rm rps; rm threaded; rm threadedandlocked;