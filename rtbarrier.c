#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


void* sleeper ( void *arg);  // prototype

pthread_barrier_t mybarrier;

//-----------------------------------

// Author: R. Tervo
//
//

void main() {

pthread_barrier_init( &mybarrier, NULL, 3);

pthread_create( NULL, NULL, &sleeper, (void *) 5 );

pthread_create( NULL, NULL, &sleeper, (void *) 10 );

pthread_create( NULL, NULL, &sleeper, (void *) 15 );

pthread_join(2,NULL);
pthread_join(3,NULL);
pthread_join(4,NULL);

}

//-----------------------------------

void* sleeper (  void *arg ) {

int time = (int) arg;

printf( "sleep(%i) is launched\n", time );

sleep(time);

pthread_barrier_wait( &mybarrier );

printf( "sleep(%i) is done\n", time );

}
