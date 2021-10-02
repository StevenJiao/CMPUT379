CC = g++

all: shell379

shell379: main.o shell379.o helperFuncs.o
	$(CC) -o shell379 shell379.o helperFuncs.o main.o

shell379.o: shell379.cpp
	$(CC) -c shell379.cpp

helperFuncs.o: helperFuncs.cpp
	$(CC) -c helperFuncs.cpp

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm -f shell379 main.o helperFuncs.o shell379.o