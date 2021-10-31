CC=gcc
FLAGS= -O -pthread

OBJECTS= mutex1 mutex2 pthread1 pthread2

all: $(OBJECTS)

mutex1: 
	$(CC) $(FLAGS) -o $@ mutex1.c

mutex2: 
	$(CC) $(FLAGS) -o $@ mutex2.c

pthread1: 
	$(CC) $(FLAGS) -o $@ pthread1.c

pthread2:
	$(CC) $(FLAGS) -o $@ pthread2.c

clean:
	rm -f $(OBJECTS)