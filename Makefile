# Makefile for echo client and echo server

CC = gcc
CFLAGS = -g -std=gnu99 -O -Wall -pthread -Wfatal-errors
DEPS = io.h client.h server.h fileReader.h broadCastServer.h broadCastClient.h socketFactory.h

all: client server

%.o: %.c ${DEPS}
	$(CC) -c -o $@ $< $(CFLAGS)

client: client.o io.o fileReader.o broadCastClient.o socketFactory.o
	$(CC) $(CFLAGS) client.o io.o fileReader.o broadCastClient.o socketFactory.o -o client 

server: server.o io.o fileReader.o broadCastServer.o socketFactory.o broadCastClient.o
	$(CC) $(CFLAGS) server.o io.o fileReader.o broadCastServer.o socketFactory.o broadCastClient.o -o server

clean:
	rm -rf *.o client server *.gch a.out*
	
