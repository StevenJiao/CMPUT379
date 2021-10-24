// Reference: https://www.geeksforgeeks.org/multithreading-c-2/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
  
// A normal C function that is executed as a thread 
// when its name is specified in pthread_create()
void* myThreadFun (void* vargp)
{
    int* num = (int *) vargp;
    printf("... In the thread function: received %d \n", *num);
    return (int*) 18;
}
   
int main()
{
    pthread_t thread_id;
    printf("-> Before Thread\n");
    // Define a variable to pass into the thread routine
    int num_to_pass = 42;
    // Define a variable to store the returned value of the thread routine
    int t_returned;
    // Creating new thread which execute myThreadFun routine, passing the variable
    pthread_create(&thread_id, NULL, myThreadFun, &num_to_pass);
    // Waiting until the thread be terminated and storing the returned value
    pthread_join(thread_id, &t_returned);
    printf("-> After Thread: returned value is %d\n", t_returned);
    exit(0);
}