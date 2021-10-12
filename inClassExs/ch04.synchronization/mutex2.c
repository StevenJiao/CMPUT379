#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// What if 100? 1,000? 10,0000?
#define	LOOP	10

#define	NO	0
#define	YES	1

// Use pAinside and pBinside to specify who is in the critical section
int pAinside, pBinside;

// Use Proc to switch between processes A and B
int Proc;

// Use Prev to see if mutex enforces alernation
int Prev;
int Alternate = YES;

// Critical section -- swap two values
int V1 =  1;
int V2 = -1;

void Swap() {
	int temp;

	if( V1 == V2 ) {
		// V1 and V2 can only have the same value if the critical
		// section is non-exclusive
		printf( "Should never happen!\n" );
		exit( 1 );
	}
        if( Proc == Prev && Alternate == YES ) {
                // Only true if Proc is not alternating between A & B
                Alternate = NO;
        }
        Prev = Proc;

	// Swap
	temp = V1;
	V1 = V2;
	V2 = temp;
}

void *threadvalue( void *arg ) {
	int i;

	printf( "Thread parent ID: %d, thread ID: %d, arg: %d\n",
		getpid(), (int)pthread_self(), (int)arg );

	// PROCESS B
	for( i = 0; i < LOOP; i++ ) {
		while( pAinside ) ;

		pBinside = YES;
		Swap(); // Critical section 
		pBinside = NO;
	}

	return( 0 );
}

int main( void ) {
	int i;
	pthread_t ntid;

	pAinside = pBinside = NO;

	if( pthread_create( &ntid, NULL, threadvalue, (void *)NULL ) ) {
		printf( "Thread create  error %d\n", (int)ntid );
		return( -1 );
	}
	printf( "Create thread %d\n", (int)ntid );

	// PROCESS A
	for( i = 0; i < LOOP; i++ ) {
		while( pBinside ) ;

		pAinside = YES;
		Swap(); // Critical section 
		pAinside = NO;
	}

	if( Alternate == YES ) {
		printf( "Critical section alternated\n" );
	} else {
		printf( "Critical section did not alternate\n" );
	}
	pthread_exit( 0 );
}
