all:	simpleexec fork threads threadssm wait

exec:	simpleexec.c
	gcc -o exec exec.c

fork:	fork.c
	gcc -o fork fork.c

threads: threads.c
	gcc -o threads threads.c

threadssm: threadssm.c
	gcc -o threadssm threadssm.c

wait:	wait.c
	gcc -o wait wait.c

clean:
	rm -f simpleexec fork threads threadssm wait
