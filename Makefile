# Makefile for echo client and echo server

CC = gcc
CFLAGS = -g -std=gnu99 -O -Wall -pthread -Wfatal-errors
DEPS = app.h client.h server.h fileReader.h broadCastServer.h socketFactory.h buffer.h

all: client server

%.o: %.c ${DEPS}
	$(CC) -c -o $@ $< $(CFLAGS)

client: client.o app.o fileReader.o broadCastServer.o socketFactory.o buffer.o
	$(CC) $(CFLAGS) client.o app.o fileReader.o broadCastServer.o socketFactory.o buffer.o -o client 

server: server.o app.o fileReader.o broadCastServer.o socketFactory.o buffer.o
	$(CC) $(CFLAGS) server.o app.o fileReader.o broadCastServer.o socketFactory.o buffer.o -o server

clean:
	rm -rf *.o client server *.gch a.out*
	
