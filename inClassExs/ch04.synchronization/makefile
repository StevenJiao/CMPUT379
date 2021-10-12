all:	mutex1 mutex2 sema

mutex1:	mutex1.c
	gcc -o mutex1 mutex1.c

mutex2:	mutex2.c
	gcc -o mutex2 mutex2.c

sema:	sema.c
	gcc -o sema sema.c -l pthread

clean:
	rm -f mutex1 mutex2 sema
