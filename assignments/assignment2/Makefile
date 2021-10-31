CC = g++
FLAGS = -O -pthread

OBJECTS = prodcon.o
TARGETS = prodcon

prodcon: $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) $(TARGETS)

$(OBJECTS): prodcon.hpp