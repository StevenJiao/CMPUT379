// This program desmonstrates the use of setjmp() and longjmp()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

// setjmp() saves state information in Restart_Looping so that lonjmp() can restore the state
jmp_buf Restart_Looping;

// SIGINT interrupt handler
// When a SIGINT (control-C) is given, have the program go back to "Restart_Looping"
void Interrupt( int signal ) {
	char answer;

	puts( "\nRestart the loop?" );
	// Read in a charcter
	answer = getchar();
	// Throw away the next character (the new-line character)
	(void) getchar();
	
	// If user says "y"es to the restart question, do a longjmp back to Restart_Looping
	if( answer == 'y' || answer == 'Y' ) {
		siglongjmp( Restart_Looping, 1 );
		// Only get to the perror() if the longjmp() fails
		perror( "Failed longjmp" );
		exit( -1 );
	}
	else {
		exit(0);
	}
}

// A do-nothing loop
void Looping() {
	int i;

	for( i = 0; i < 100; i++ ) {
		printf( "%d ", i );
		fflush( stdout );
		sleep( 1 );
	}
}

// Try typing control-C while this program is running. The longjmp() takes the program back
// from being inside Looping (or even being inside of printf, inside of Looping) to Restart_Loopoing
int main( void ) {


	// Catch control-C
	signal( SIGINT, Interrupt );
	// Save the state for a potential longjmp() back to here
	if( sigsetjmp( Restart_Looping,1 ) < 0 ) {
		perror( "Failed setjmp" );
		exit( -1 );
	}
	// Idle computing
	Looping();

	return( 0 );
}
