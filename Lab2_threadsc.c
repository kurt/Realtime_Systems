
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***
------------------------------------------------
threads.c    R.Tervo       Oct 2008
------------------------------------------------
A simple example of creating a thread.
The thread prints the uppercase alphabet.
while the main program prints a lowercase alphabet.
------------------------------------------------
 compile using  -lpthread
------------------------------------------------
***/

void checkresult( int result, char *text );
static void *myfunction( void *args );
static void *random_function( void *args );

//==============================================
main()
{
int   result;
char *p;

pthread_t tid;
pthread_t tid2;
 result = pthread_create( &tid, NULL, myfunction, NULL );
 checkresult( result, "thread create failed" );
 result = pthread_create(&tid2, NULL, random_function, NULL);
 checkresult(result, "thread create failed");
 pthread_setschedprio(tid2,9);

 printf("The main's PID is: %d\n",getpid());
 printf("The main's PPID is: %d\n",getppid());
 for(p="abcdefghijklmnopqrstuvwxyz\n"; *p; p++ )
 { sleep(1);
   putchar(*p);
   fflush(stdout);
 }

 result = pthread_join( tid, NULL );
 checkresult( result, "thread create failed" );

 exit(0);
} // end main

//==============================================

static void *myfunction( void *args )
{
	 printf("The myfunctions's PID is: %d\n",getpid());
	 printf("The myfunction's PPID is: %d\n",getppid());
	struct sched_param param;
	pthread_getschedparam(gettid(), NULL, &param);
	printf("Consumer assigned priority: %d\n", param.sched_priority);
	printf("Consumer current priority: %d\n", param.sched_curpriority);
 char *p;

 for(p="ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"; *p; p++ )
 { putchar(*p);
   fflush(stdout);
   sleep(1);
 }
}
//==============================================

static void *random_function(void *args){
	 printf("The random_function's PID is: %d\n",getpid());
	 printf("The random_function's PPID is: %d\n",getppid());
	struct sched_param param;
	pthread_getschedparam(gettid(),NULL, &param);
	printf("Consumer assigned priority: %d\n", param.sched_priority);
	printf("Consumer current priority: %d\n", param.sched_curpriority);
	int i;
	for(i=0;i<26;i++){
		printf("%d",i);
		//fflush(stdout);
		sleep(1);
	}// end of for
}

//==============================================

void checkresult( int result, char *text )
{
if( result == -1 )
{
 perror(text);
 exit(1);
}

} // end check result
