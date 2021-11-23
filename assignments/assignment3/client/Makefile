CC = g++
FLAGS = -O

OBJECTS = client.o

client: $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) client