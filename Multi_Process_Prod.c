/***
forker.c        R.Tervo     Jan 2013
-------------------------------------
Exploring fork() and exec()
This program monitors PID and PPID in
while spawning a second program.
-------------------------------------
***/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/neutrino.h>
#define MSGSIZE 80
int chid;
int rcvid;
unsigned flags;
char send_message[MSGSIZE]; 		//send message
char receive_message[MSGSIZE]; 		//receive message
char reader[MSGSIZE]; //
char readerreply[MSGSIZE];




main( )
{
	pid_t pidA, ppidA;       // define PID storage
	pid_t pidB, ppidB;
	pid_t pidC,  pid;
	pid_t pidD, pidE;
	pid_t ppidE;

int   status;		 // status of wait()

puts("Embedded Message Passing Example Program");
puts("creating channel");
	flags = NULL;
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

pidC  = fork();	         // returns child PID (or 0 if child)

pidB  = getpid();
ppidB = getppid();



if(pidA != pidB)
 {
   printf("PID=%i, PPID=%i  I am the child.  \n", pidB, ppidB);

   printf("Exec now..\n");
   fflush(stdout);          // complete any pending prints
      int ucoid;
      	puts("user has been called");
      	puts("user connecting to channel. \n");
      	ucoid = ConnectAttach(0, pidA, chid, 0, NULL);
      	while(1) {
      		puts("user:go ahead, enter a string");
      		gets(send_message);
      		printf("user:waiting to send: = %s =.\n",send_message);
      		MsgSend(ucoid, &send_message, sizeof(send_message), &receive_message, sizeof(receive_message));
      		printf("user: received a reply from friend: = %s =\n", receive_message);
      		sleep(1);
      	}
 }
else
 {
	 pidD= fork();
	 pidE=getpid();
	 ppidE=getppid();
	 if(pidA != pidE){
		 printf("PID=%i, PPID=%i  I am the parent.  \n", pidA, ppidA);

		 		   printf("Exec now..\n");
		 		   fflush(stdout);          // complete any pending prints
		 		   int prodcoid;
		 			puts("prod has been called");
		 			puts("prod connecting to channel. \n");
		 			prodcoid = ConnectAttach(0, pidA, chid, 0, NULL);
		 			while(1) {
		 				puts("prod:go ahead, enter a string");
		 				//gets(send_message);
		 				strcpy(send_message,"hello");
		 				printf("prod:waiting to send: = %s =.\n",send_message);
		 				MsgSend(prodcoid, &send_message, sizeof(send_message), &receive_message, sizeof(receive_message));
		 				printf("prod: received a reply from friend: = %s =\n", receive_message);
		 				sleep(1);
	 }
	 }
	 else{
		 //--
		 printf("PID=%i, PPID=%i  I am the second child.  \n", pidE, ppidE);
		 		  int fcoid;
		 		  puts("friend has been called");
		 		  puts("friend connecting to channel. \n");
		 		  fcoid = ConnectAttach(0, pidA, chid, 0, NULL);
		 		  strcpy(readerreply, "Got it!");
		 		  while(1) {
		 	 		printf("friend: I am waiting to receive message from user.\n");
		 	 		rcvid = MsgReceive(chid, &reader, sizeof(reader), NULL);
		 	 		printf("friend: received = %s = from user.\n", reader);
		 	 		MsgReply(rcvid, NULL, &readerreply, sizeof(readerreply));
		 	 		sleep(1);
		 		   printf("PID=%i, PPID=%i  I am the parent. \n", pidB, ppidB);
		 		 }
		 		pid = wait(&status);	 // wait for child process to end (none)
		 		printf("I am done waiting for PID[%i] with status %i\n",pid, status);
		 //--

	   //execl("./hello","hello",'\0');  // replace this process with another

	   printf(" ...we never print this line2...\n");
	 }

 }
return(0);
} // end main
