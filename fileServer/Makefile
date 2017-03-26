# Makefile for echo client and echo server

CC = g++
CFLAGS = -g -std=c++11 -O -Wall -pthread -Wfatal-errors -Wno-parentheses -Wno-write-strings
DEPS = app.h packetSocket.h socketFactory.h buffer.h flags.h
ODEPS = app.o packetSocket.o socketFactory.o buffer.o

all: client server

%.o: %.cpp ${DEPS}
	$(CC) -c -o $@ $< $(CFLAGS)

client: client.o ${ODEPS}
	$(CC) client.o $(CFLAGS) ${ODEPS} -o client 

server: server.o ${ODEPS}
	$(CC) server.o $(CFLAGS) ${ODEPS} -o server

clean:
	rm -rf *.o client server *.gch a.out*
	
