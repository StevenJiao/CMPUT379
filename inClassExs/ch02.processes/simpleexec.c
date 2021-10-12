// Example of a fork()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( void ) {
	pid_t pid;
	char * args[2] = {"date", NULL};

	if( fork() ) {	// Parent
		printf( "It is today... " );
	} else {	// Child
		printf( "The date is... " );
		if( execvp( args[0], args ) < 0 )
			perror( "Exec problem:" );
	}
	printf( "...but soon will be tomorrow\n" );
	return( 0 );
}

// Question 1: "The date is...". Why is it missing?
// Question 2: What if an invalid command is passed to exec?
