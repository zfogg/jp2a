C=gcc
CFLAGS=-c -Wall

all: client server

client: client.o
	$(CC) client.o -o client.out

server: server.o
	$(CC) server.o -o server.out

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -rf *.o

reset:
	rm -rf *.o *.out && make