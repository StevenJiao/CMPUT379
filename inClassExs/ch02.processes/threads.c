// Example of creating threads

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NTHREADS 5

// Function executed by each thread
void *threadvalue( void *arg ) {
	printf( "Parent ID: %d, thread ID: %d, arg: %d\n",
		getpid(), (int)pthread_self(), (int)arg );
	return( 0 );
}

int main( void ) {
	int i;
	pthread_t ntid;

	for( i = 0; i < NTHREADS; ++i ) {
		if( pthread_create( &ntid, NULL, threadvalue, (void *)i ) ) {
			perror( "Thread create" );
		}
		printf( "Create thread %d\n", (int)ntid );
	}
	pthread_exit( 0 );
}
