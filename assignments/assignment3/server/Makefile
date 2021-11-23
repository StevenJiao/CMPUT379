CC = g++
FLAGS = -O -pthread

OBJECTS = multiserver.o

multiserver: $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f multiserver $(OBJECTS)