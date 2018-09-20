// 	Program:	prod-conc.txt
//	Author:	Generic producer consumer code set up by M. E. //				Kaye 2014
//	Class:	ECE 4251
//	OS:		QNX 6.5.1
//	Date:		September 17, 2014

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int policy;

// function producer

void* producer(void)
{
	struct sched_param param;
	sched_getparam(getpid(), &param);
	 	printf("Consumer assigned priority: %d\n", param.sched_priority);
	 	printf("Consumer current priority: %d\n", param.sched_curpriority);
/*	Initialize all the variables	*/
         int i;
  //        int true = 1;
          char rmsg[]= "here we go";

          printf ("Producer attached to the system %d %d\n ", getpid(), gettid());

      		for(i=1;i<6;i++)
                     {
                   delay(500);

                   printf("The producer runs again %s.\n",rmsg);
                     }

          return(0);
}

// function consumer
void* consumer(void)
{
	struct sched_param param;
	sched_getparam(getpid(), &param);
	 	printf("Producer assigned priority: %d\n", param.sched_priority);
	 	printf("Producer current priority: %d\n", param.sched_curpriority);
/*	Prepare any variables used	*/

          char smsg[] = "Yes, Yes";

           int i;
 //         int true = 1;


          printf ("Consumer attached to the system. %d %d\n ", getpid(), gettid());

              /* delay program to allow other process to run */
          delay(1000);


          for(i=1;i<6;i++)
          {
                   delay(250);

                   printf("Consumer runs again %s\n",smsg);

          } /* for loop */
          return(0);
}
void checkresult( int result, char *text )
{
if( result == -1 )
{
 perror(text);
 exit(1);
}

} // end check result


	int main(int argc, char *argv[]) {
		printf("Welcome to the QNX Momentics IDE\n");
//		return EXIT_SUCCESS;
		/*	Start the two threads producer and consumer		*/
	int result;
	struct sched_param param;
	struct sched_param param_2;
	pthread_t consid;
	pthread_t prodid;
//	pthread_attr_t attr;
//	pthread_attr_init(&attr);
 	result = pthread_create(&consid, NULL, &consumer, NULL);
 	checkresult( result, "consumer create failed" );
 	result = pthread_create(&prodid, NULL, &producer, NULL);
 	checkresult( result, "producer create failed" );
 	//sched_getparam(2, &param);
 	//printf("Consumer assigned priority: %d\n", param.sched_priority);
 	//printf("Consumer current priority: %d\n", param.sched_curpriority);
 	//sched_getparam(3, &param_2);
 	//printf("Producer assigned priority: %d\n", param.sched_priority);
 	//printf("Producer current priority: %d\n", param.sched_curpriority);

 //	pthread_attr_getschedpolicy(&attr, &policy);
 //       printf("Scheduling policy: %d\n", sched_getscheduler(0));
	sleep(15);
     return (0);
}

