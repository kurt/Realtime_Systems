#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

// 
#define SKED SCHED_FIFO 
//#define SKED SCHED_RR 

void* looper (void *arg);  // prototype

sem_t mysem;

//-----------------------------------
// THREAD PRIORITY DEMO - RTervo Jan 2013
//-----------------------------------
// 
//-----------------------------------

main() {

int tid1, tid2, tid3;

pthread_attr_t attr1, attr2, attr3;
struct sched_param  param;

pthread_attr_init( &attr1); // assign default attributes
pthread_attr_setinheritsched( &attr1, PTHREAD_EXPLICIT_SCHED );
pthread_attr_setschedpolicy(  &attr1, SKED );

pthread_attr_init( &attr2); // assign default attributes
pthread_attr_setinheritsched( &attr2, PTHREAD_EXPLICIT_SCHED );
pthread_attr_setschedpolicy(  &attr2, SKED );

pthread_attr_init( &attr3); // assign default attributes
pthread_attr_setinheritsched( &attr3, PTHREAD_EXPLICIT_SCHED );
pthread_attr_setschedpolicy(  &attr3, SKED );

int pid = getpid();
printf( "my pid is [%i]\n", pid );

sem_init( &mysem, 1, 3 ); // allow N users

pthread_attr_getschedparam (  &attr1, &param );
param.sched_priority=  7; // modify thread priority
pthread_attr_setschedparam (  &attr1, &param );

pthread_attr_getschedparam (  &attr2, &param );
param.sched_priority=  7;  // modify thread priority
pthread_attr_setschedparam (  &attr2, &param );

pthread_attr_getschedparam (  &attr3, &param );
param.sched_priority=  7;  // modify thread priority
pthread_attr_setschedparam (  &attr3, &param );

pthread_create( &tid1, &attr1, &looper, (void *) 1 );
pthread_create( &tid2, &attr2, &looper, (void *) 2 );
pthread_create( &tid3, &attr3, &looper, (void *) 3 );

pthread_join(2,NULL);
pthread_join(3,NULL);
pthread_join(4,NULL);

} // end main

//-----------------------------------


void* looper (  void *arg ) {

int tag = (int) arg;
int x = 0;
unsigned int y = 0;

printf( "looper(%i) started\n", tag );

while( x < 10 ) {  // do ten loops

sem_wait(&mysem);

 printf( "looper(%i) t=%i\n", tag, x++ );

 for(y=0;y<0x200000;y++); // loop delay

// sched_yield();  // optional yield

// delay(10);      // sleep by msec 

sem_post(&mysem);

} // end while

printf( "looper(%i) is done\n", tag );

} // end looper

//-----------------------------------