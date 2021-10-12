// Create a parent and child process

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( void ) {
	pid_t pid;

	if( fork() < 0 ) {
		perror( "Could not fork\n" );
	} else {
		// Parent and child execute this code
		pid = getpid();
		printf( "I am %d!\n", pid );
	}
	return( 0 );
}
