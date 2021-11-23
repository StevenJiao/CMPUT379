CC = g++
FLAGS = -O -pthread

OBJECTS = client.o server.o
TARGETS = client server

all: client server

client: client.o
	$(CC) $(FLAGS) -o $@ $^

server: server.o
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) $(TARGETS)