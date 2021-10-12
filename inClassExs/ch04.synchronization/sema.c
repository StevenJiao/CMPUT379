#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define	NTHREADS 2

// What if 100? 1,000? 10,0000?
#define	LOOP	10

#define	NO	0
#define	YES	1

// Critical section -- swap two values
int V1 =  1;
int V2 = -1;

// Binary semaphore
sem_t SwapLock;

void Swap() {
	int temp;

	sem_wait( &SwapLock );
	if( V1 == V2 ) {
		// V1 and V2 can only have the same value if the critical
		// section is non-exclusive
		printf( "Should never happen!\n" );
		exit( 1 );
	}

	// Swap
	temp = V1;
	V1 = V2;
	V2 = temp;
	sem_post( &SwapLock );
}

void *threadloop( void *arg ) {
	int i;

	printf( "Start thread\n" );
	for( i = 0; i < LOOP; i++ ) {
		Swap(); // Critical section 
	}
	printf( "End thread\n" );
	return( 0 );
}

int main( void ) {
	int i;
	pthread_t ntid;

	if( sem_init( &SwapLock, 0, 1 ) ) {
                perror( "Sempahore init" );
                exit( -1 );
        }

	for( i = 0; i < NTHREADS; i++ ) {
		if( pthread_create( &ntid, NULL, threadloop, (void *)NULL ) ) {
			perror( "Thread create" );
			return( -1 );
		}
	}
	pthread_exit( 0 );
}
