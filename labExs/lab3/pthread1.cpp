// Reference: https://www.geeksforgeeks.org/multithreading-c-2/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
  
// A normal C function that is executed as a thread 
// when its name is specified in pthread_create()
void* myThreadFun (void* vargp)
{
    sleep(1);
    printf("In the thread function: Hello :D \n");
    return NULL;
}
   
int main()
{
    pthread_t thread_id;
    printf("Before Thread\n");
    // Creating new thread which execute myThreadFun routine
    pthread_create(&thread_id, NULL, myThreadFun, NULL);
    // Waiting until the thread be terminated
    pthread_join(thread_id, NULL);
    printf("After Thread\n");
    exit(0);
}