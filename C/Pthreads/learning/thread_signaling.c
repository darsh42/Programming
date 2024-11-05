#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

pthread_cond_t notify = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_signal( void * ignore );
void *thread_wait_signal( void * ignore );

int main( void )
{
    pthread_t wait;
    pthread_t signal;

    assert(!pthread_create(&wait, NULL, thread_wait_signal, NULL));
    
    sleep(2);

    assert(!pthread_create(&signal, NULL, thread_signal, NULL));

    pthread_join(wait, NULL);
    pthread_join(signal, NULL);
}

void *thread_signal( void * ignore )
{
    printf("thread: %ld\n", pthread_self());
    printf("sending signal\n");
    assert(!pthread_cond_signal(&notify));
}

void *thread_wait_signal( void * ignore )
{
    printf("thread: %ld\n", pthread_self());
    printf("waiting signal\n");

    assert(!pthread_cond_wait(&notify, &mutex));

    printf("recived signal\n");
}
