#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// What if this number is bigger?
#define NLINES 4

int i, status;
pid_t pid;

int main( void ) {

	pid = fork();
	if( pid < 0 ) {
		perror( "Fork problem" );
	}
	else if( pid == 0) {
		// Child process
		for( i = 0; i < NLINES; i++ )
			printf( "%d child!\n", i );
	} else {
		// Parent process
		for( i = NLINES; i > 0; i-- )
			printf( "%d parent!\n", i );
		// Wait for all children to finish before continuing
		wait( &status );
		printf( "Parent done!\n" );
	}
	return( 0 );
}

// What if NLINES is bigger?
// Does the output differ if there is no wait()? Why?
