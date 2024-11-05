#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

void *mutex_function( void *params );

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main ( void )
{
    pthread_t thread_a, thread_b;

    assert(pthread_create(&thread_a, NULL, &mutex_function, NULL) == 0);
    assert(pthread_create(&thread_b, NULL, &mutex_function, NULL) == 0);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    exit(0);
}

void *mutex_function( void * params )
{
    pthread_mutex_lock( &mutex );
    printf("Counter value: %d\n", ++counter);
    pthread_mutex_unlock( &mutex );
}
