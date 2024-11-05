#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define NTHREADS 10

void *mutex_function( void *params );

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main ( void )
{
    pthread_t threads[NTHREADS];

    for(int i = 0; i < NTHREADS; i++)
    {
        assert(pthread_create( &threads[i], NULL, &mutex_function, NULL ) == 0);
    }
    
    for(int i = 0; i < NTHREADS; i++)
    {
        pthread_join( threads[i], NULL );
    }

    exit(0);
}

void *mutex_function( void * params )
{
    printf("thread number: %ld\n", pthread_self());
    pthread_mutex_lock( &mutex );
    printf("Counter value: %d\n", ++counter);
    pthread_mutex_unlock( &mutex );
}
