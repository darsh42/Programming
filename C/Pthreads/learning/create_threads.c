#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function( void *parameter );

int main( void )
{
    pthread_t thread_a, thread_b;
    const char *message_a = "Thread A";
    const char *message_b = "Thread B";
    
    int result_a, result_b;

    result_a = pthread_create( &thread_a, NULL, print_message_function, (void *) message_a);
    result_b = pthread_create( &thread_b, NULL, print_message_function, (void *) message_b);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    printf("Thread A: %d\n", result_a);
    printf("Thread B: %d\n", result_b);

    exit(0);
}

void *print_message_function( void * parameter )
{
    const char *message = (const char *) parameter;
    printf("%s\n", message);
}
