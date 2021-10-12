// Example of threads accessing shared memory

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NTHREADS 5

// Global variable
int Count = 0;


// Function executed by each thread
void *threadvalue( void *arg ) {
	printf( "Parent ID: %d, thread ID: %d, arg: %d\n",
		getpid(), (int)pthread_self(), (int)arg );
	// Count is common to all threads (and the parent process)
	Count += (int) arg;
	Count *= (int) arg;
	printf( "Execute thread %d. Count = %d\n", (int)arg, Count );
	return( 0 );
}

int main( void ) {
	int i;
	pthread_t ntid;

	for( i = 0; i < NTHREADS; ++i ) {
		if( pthread_create( &ntid, NULL, threadvalue, (void *)i ) ) {
			perror( "Thread create" );
		}
		printf( "Create thread %d. Count = %d\n", (int)ntid, Count );
	}
	// Because of the non-deterministic execution of the threas, Count can
	// take on different values
	printf( "Exit with Count = %d\n", Count );
	pthread_exit( 0 );
}
