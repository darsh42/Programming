/*******************************************************************************************************
 *                        multithreaded snake game using ncurses and pthreads                          *
 *******************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

#include <pthread.h>
#include <ncurses.h>

enum dir_e {
    DIR_UP,
    DIR_RH,
    DIR_DW,
    DIR_LH
};

/* flag telling when it is ok to draw */
bool drawing = false;

/* snake positions */
uint64_t   snake_pos_x; pthread_mutex_t mutex_pos_x    = PTHREAD_MUTEX_INITIALIZER;
uint64_t   snake_pos_y; pthread_mutex_t mutex_pos_y    = PTHREAD_MUTEX_INITIALIZER;    
enum dir_e snake_dir;   pthread_mutex_t mutex_pos_dir  = PTHREAD_MUTEX_INITIALIZER;

/* 64 x 64 board with each bit representing either empty, 0, or filled, 1 */
uint64_t board[] = 
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
pthread_mutex_t mutex_board;
/*******************************************************************************************************
 *                                      snake program thread                                           *
 *******************************************************************************************************
 *      This function will keep the snake moving and process any moves/inputs affecting the snake.     *
 *      Currently it will contain the main backend data and only expose some interface to the io       *
 *      thread allowing the user to move the snake                                                     *
 *******************************************************************************************************/

uint64_t read_pos_x( void ) { 
    uint64_t ret;

    pthread_mutex_lock(mutex_pos_x);
    ret = snake_pos_x;
    pthread_mutex_unlock(mutex_pos_x);

    return ret;
}

void *thread_function_snake(void *args)
{
    /* initialisation of snake game */
    pthread_mutex_lock(mutex_pos_x);
    pthread_mutex_lock(mutex_pos_y);
    pthread_mutex_lock(mutex_dir);

    snake_pos_x = 0;        // top  most
    snake_pos_y = 0;        // left most
    snake_dir   = DIR_RH;   // direction right

    pthread_mutex_unlock(mutex_pos_x);
    pthread_mutex_unlock(mutex_pos_y);
    pthread_mutex_unlock(mutex_dir);

    for ( ;; ) 
    {
        
    }

cleanup:

    pthread_exit(NULL);
}

/*******************************************************************************************************
 *                                        input output thread                                          *
 *******************************************************************************************************
 *      This function will update and look for any user inputs, this will be done through ncurses      *
 *      
 *******************************************************************************************************/
void *thread_function_io(void *args)
{
    /** start NCURSES */
    initscr();          // initialize the ncurses screen
    raw();              // set the input type to raw
    curs_set(0);        // set the cursor to the top left
    start_color();      // start the color subsystem
    
    for ( ;; )
    {

    }

cleanup:
    endwin();
    pthread_exit(NULL);
}

/** entry point */
int main(void)
{
    int rc;
    
    pthread_t thread_io;     // manages IO and ncurses
    pthread_t thread_snake;  // manages main program logic

    pthread_attr_t attr_io    = {};   // IO attributes
    pthread_attr_t attr_snake = {};   // snake attributes

    rc = pthread_attr_setdetachstate(&attr_io,    PTHREAD_CREATE_JOINABLE); assert(!rc); // IO thread is not joinable
    rc = pthread_attr_setdetachstate(&attr_snake, PTHREAD_CREATE_JOINABLE); assert(!rc); // snake thread is not joinable

    rc = pthread_create(&thread_io,    &attr_io,    thread_function_io,    NULL); assert(!rc);
    rc = pthread_create(&thread_snake, &attr_snake, thread_function_snake, NULL); assert(!rc);

    pthread_exit(NULL);
}
