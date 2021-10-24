#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t milk_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;
bool no_milk = false;
int num_milk = 1;

void *arthur()
{
   for(;;)
   {
      pthread_mutex_lock( &condition_mutex );
      while( num_milk > 0)
      {
         pthread_cond_wait( &condition_cond, &condition_mutex );
      }
      pthread_mutex_unlock( &condition_mutex );

      pthread_mutex_lock( &milk_mutex );
      num_milk++;
      no_milk = false;
      printf("Arthor buy milk: %d\n",num_milk);
      pthread_mutex_unlock( &milk_mutex );
      sleep(3);
    }
    return(NULL);
}

void * peter()
{
    for(;;)
    {
       pthread_mutex_lock( &condition_mutex );
       if( num_milk <= 0 )
       {
          pthread_cond_signal( &condition_cond );
          no_milk = true;
       }
       pthread_mutex_unlock( &condition_mutex );
      
       if(no_milk) continue;
      
       pthread_mutex_lock( &milk_mutex );
       num_milk--;
       printf("Peter drink milk: %d\n",num_milk);
       pthread_mutex_unlock( &milk_mutex );
       sleep(3);
    }
    return(NULL);

}

main()
{
   pthread_t thread1, thread2;

   pthread_create( &thread2, NULL, &arthur, NULL);
   pthread_create( &thread1, NULL, &peter, NULL);
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   exit(0);
}