CC = g++
FLAGS = -O -pthread

OBJECTS = server.o

server: $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f server $(OBJECTS)