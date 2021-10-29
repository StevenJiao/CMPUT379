CC = g++
FLAGS = -O -pthread

OBJECTS = prodcon.o

prodcon: $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f prodcon $(OBJECTS)

$(OBJECTS): prodcon.hpp