CC = gcc

CFLAGS = -std=c99 -Wall

all:
	$(CC) -o gameserver379 server.c -lpthread -lm
	$(CC) $(CFLAGS) -o gameclient379 client.c -lncurses

clean:
	rm -rf *o gameserver379
	rm -rf *o gameclient379
