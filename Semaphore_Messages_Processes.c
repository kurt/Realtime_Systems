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
#define TIME_PERIOD 20
#define MSGSIZE NULL

// change the size to change the message size passed
#define SIZE 32

// Semaphore Definitions
sem_t sem_parent_wait;
sem_t sem_child_wait;

int N = 0;
int nbytes;
int chid;
int rcvid;
unsigned flags;
char send_message[MSGSIZE]; 		//send message
char receive_message[MSGSIZE]; 		//receive message
char reader[MSGSIZE]; //
char readerreply[MSGSIZE];


pid_t pidA, ppidA;       // define PID storage
pid_t pidB, ppidB;
pid_t pidC,  pid;

void timer_expired(int called_via_signal)
{
    printf("%d iterations in %i seconds\n", N, TIME_PERIOD);
    exit(0);
}


main(){
     extern void timer_expired();
     int i, d, nbytes, cps, status;
     char *addr;
     int n,m;
     n=0;
     m=0;
     time_t time_a;
     time_t time_b;
     double wake_up_time;

     sem_init(&sem_parent_wait,0,1);
     sem_init(&sem_child_wait,0,0);

     // setup alarm
     struct sigaction sa;

    // Set up signal handler.
          sa.sa_handler = timer_expired;
          sa.sa_flags = 0;
               sigemptyset(&sa.sa_mask);
               if (sigaction(SIGALRM, &sa, NULL) < 0) {
                   perror("sigaction SIGALRM.\n");
                   exit(1);
                }


     //Create Channel
     //Create child parent
     //fork to create child process
     chid = ChannelCreate(flags);
     	if (chid==1){
     		printf("could not create channel \n");
     		//exit(1);
     	}

     pidA  = getpid();        // establish main program PID
     ppidA = getppid();
     printf("PID=%i, PPID=%i  I am the main program.\n", pidA, ppidA);
     pid = wait(&status);	 // wait for child process to end (none)
     printf("I am done waiting for PID[%i] with status %i\n",pid, status);

     printf("Fork now..\n");
     fflush(stdout);          // complete any pending prints
     alarm(TIME_PERIOD);
     pidC  = fork();	         // returns child PID (or 0 if child)

     pidB  = getpid();
     ppidB = getppid();

     if(pidA != pidB)
      {
        printf("PID=%i, PPID=%i  I am the child.  \n", pidB, ppidB);
        printf("Exec now..\n");
          fflush(stdout);          // complete any pending prints
          int child_attachID;
          	puts("user has been called");
          	puts("user connecting to channel. \n");
          	child_attachID = ConnectAttach(0, pidA, chid, 0, NULL);
          	while(1) {
          		sem_wait(&sem_child_wait);
          		n++;
          		puts("user:go ahead, enter a string");
          		gets(send_message);
          		printf("user:waiting to send: = %s =.\n",send_message);
          		MsgSend(child_attachID, &send_message, sizeof(send_message), &receive_message, sizeof(receive_message));
          		printf("user: received a reply from friend: = %s =\n", receive_message);
          		sem_post(&sem_parent_wait);
          	}
          //execl("./hello","hello",'\0');  // replace this process with another

          printf(" ...we never print this line...\n");\
      } else
      {
     	 int fcoid;
     	 	puts("friend has been called");
     	 	puts("friend connecting to channel. \n");
     	 	fcoid = ConnectAttach(0, pidB, chid, 0, NULL);
     	 	strcpy(readerreply, "Got it!");
     	 	while(1) {
     	 		sem_wait(&sem_parent_wait);
     	 		m++;
     	 		printf("friend: I am waiting to receive message from user.\n");
     	 		rcvid = MsgReceive(chid, &reader, sizeof(reader), NULL);
     	 		printf("friend: received = %s = from user.\n", reader);
     	 		MsgReply(rcvid, NULL, &readerreply, sizeof(readerreply));
     			printf("PID=%i, PPID=%i  I am the parent. \n", pidB, ppidB);
     			sem_post(&sem_child_wait);
     		}

     	pid = wait(&status);	 // wait for child process to end (none)
     	printf("I am done waiting for PID[%i] with status %i\n",pid, status);
      }
     //make semaphores










}

