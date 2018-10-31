//  EE4273 Sample Source Code. SHMSEM1.C
//
//  This source code implements a single process that benchmarks the time
//  it takes to update a block of shared memory that is protected with a
//  POSIX .4 semaphore (as a means for mutual exclusion). There is only
//  one process and we are benchmarking the cost of mutual exclusion. In
//  this case the majority of accesses are uncontested. The program may
//  be run with the semaphore locks included and then excluded, (i.e.,
//  "commented out") so the resulting semaphore overhead may be measured.
//
//  The time measurements are made using the following procedure. The
//  shared memory is updated continually together with the number of
//  iterations. An ALARM is set to generate a signal (interrupt) after a
//  fixed interval of time expires. The signal handler displays the value
//  of the count. Perform this test for two cases: 1) when semaphore
//  locks are included and 2) not included.
//
//  Alternatively you could fork a child and run two processes at once
//  benchmarking total throughput.

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
#include <math.h>
#include <float.h>
#include <signal.h>
#include <time.h>

#define SHMNAME "/my_shm"
#define SEMNAME "/my_sem"
#define TRUE 1
#define TIME_PERIOD 1

// change the size to change the message size passed
#define SIZE 32


int N = 0;
int nbytes;

void timer_expired(int called_via_signal)
{
    printf("%d iterations in %i seconds\n", N, TIME_PERIOD);
    exit(0);
}


main(){
     extern void timer_expired();
     int i, d, nbytes, cps;
     char *addr;
     time_t time_a;
     time_t time_b;
     double wake_up_time;



     //timer_t sem_wake_time;
     //struct sigevent evp;
     //evp.sa=NULL;
     struct sigaction sa;

     //int timer_create( CLOCK_SOFTTIME , NULL, sem_wake_time );

     // The following structure is overlaid on the shared memory.
     struct phu {
          sem_t s;
          char beginning_of_data[SIZE];
     } *p;

     nbytes = sizeof(*p);

     // Set up signal handler.
     sa.sa_handler = timer_expired;
     sa.sa_flags = 0;
          sigemptyset(&sa.sa_mask);
          if (sigaction(SIGALRM, &sa, NULL) < 0) {
              perror("sigaction SIGALRM.\n");
              exit(1);
           }

     // Create shared memory region.
     if ((d = shm_open(SHMNAME, O_RDWR|O_CREAT|O_EXCL, 0666)) == -1) {
          printf("Unable to open shared memory.\n");
          exit(1);
     }
     if (ftruncate(d, nbytes) != 0) {
          close(d);
          shm_unlink(SHMNAME);
          printf("Unable to truncate.\n");
          exit(1);
     }
     p = (struct phu *)mmap(NULL, nbytes, PROT_READ|PROT_WRITE, MAP_SHARED, d,
0);
     if(p == (struct phu *) -1) {
          close(d);
          printf("Unable to mmap.\n");
          exit(1);
     }

     shm_unlink(SHMNAME);

     // Create semaphore.

     if (sem_init(&p->s, 1, 1) == -1) {
          printf("Sema init ERROR = %i.\n", errno);
          exit(1);
     }


     // Begin test - repeatedly acquire mutual exclusion, write to area and
     // release mutual exclusion.

     addr = p->beginning_of_data;
     N = 0;

     alarm(TIME_PERIOD);

     while ( TRUE ) {
    	// time_a = clock();
          // Acquire parents lock.
          if (sem_wait(&p->s) == -1) {
              printf("Sem_wait error.\n");
          }
          //time_b = clock();
          //wake_up_time=1e9*difftime(time_b,time_a);
          //printf( "Wake up time is: %lf \n\r", wake_up_time );
         // Store data in shared memory.
          /*
          for (i = 0; i < SIZE; i++) {
               addr[i] = 'A';
 //              printf("i = %d. N = %d.\n", i, N);
   // Uncomment this line to debug
          }
*/
          // Release parents lock.
          if (sem_post(&p->s) == -1) {
              printf("Sem_post error.\n");
           }


          N++;
     }

}

