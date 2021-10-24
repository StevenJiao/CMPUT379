// Reference: https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

void *myThreadFun(void* var)
{
    pthread_mutex_lock( &mutex1 );
    int* time = (int *) var;
    counter++;
    sleep(*time);
    printf("Counter value: %d\n",counter);
    pthread_mutex_unlock( &mutex1 );
}

void main()
{
   int rc1, rc2;
   pthread_t thread1, thread2;

    int sleep_1 = 1;
    int sleep_2 = 2;
   /* Create independent threads each of which will execute myThreadFun */

   if( (rc1=pthread_create( &thread1, NULL, &myThreadFun, &sleep_1)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }

   if( (rc2=pthread_create( &thread2, NULL, &myThreadFun, &sleep_2)) )
   {
      printf("Thread creation failed: %d\n", rc2);
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL); 

   exit(0);
}