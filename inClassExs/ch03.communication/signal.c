// This program demonstrates how to add a signal handler

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

// Global variable used to show where the program is when the interrupt occurs
int Counter;

// SIGINT interrupt handler
// When a SIGINT (control-C) is given, print out a message and then continue
void Interrupt( int signal ) {
	fprintf( stderr, "\nInterrupt! Caught signal %d (Counter=%d)\n", signal, Counter );
}

// SIGQUIT interrupt handler
// When a SIGQUIT (control-\) is given, print out a message and then end the program
void Quit( int signal ) {
	fprintf( stderr, "\nInterrupt! Caught signal %d (Counter=%d)\n", signal, Counter );
	exit( 0 );
}

// control-z
void testing(int signal) {
	fprintf(stderr, "\nayo u hit this shiet! Caught signal %d (Counter=%d)\n", signal, Counter);
}

int main (void) {
	// When a SIGINT (control-C) is given, call the routine Interrupt()
	signal( SIGINT, Interrupt );
	// When a SIGQUIT (control-\) is given, call the routine Quit()
	signal( SIGQUIT, Quit );
	//control z
	signal( SIGTSTP, testing );

	// A do-nothing loop to illustrate when signals occur
	for( Counter = 0; Counter < 100; Counter++ ) {
		putchar( '.' );
		fflush( stdout ); // Note what happens if you do not include this
		sleep( 1 );
	}
	return( 0 );
}
