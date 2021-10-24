CC = g++

prodcon: prodcon.o tands.o
	$(CC) -pthread -o prodcon prodcon.o tands.o

clean:
	rm -f prodcon prodcon.o tands.o