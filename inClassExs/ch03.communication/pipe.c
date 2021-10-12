// This program demonstrates using a pipe

#include <stdio.h>

// This program opens a pipe to "sort" the file pipe.c
// To see it work, put this code in a file pipe.c
int main (void) {
	FILE * pfile;
	int c;

	// Open the pipe. The pipe's output (i.e., the results of the "sort" command
	// will be available for reading by this program (accessible as FILE "pfile")
	if( ( pfile = popen( "sort pipe.c", "r" ) ) == NULL ) {
		perror( "Pipe open failure\n" );
	} else {
		printf( "START: print lines of text in pipe.c sorted\n" );

		// Read the the pipe until end-of-file is reached.
		while( ( c = fgetc( pfile ) ) != EOF ) {
			if( putchar( c ) == EOF ) {
				perror( "Putchar failure\n" );
				return( -1 );
			}
		}
		printf( "END: print lines of text in pipe.c sorted\n" );
		if( pclose( pfile ) ) {
			perror( "Pipe close failure\n" );
		}
	}
	return( 0 );
}
