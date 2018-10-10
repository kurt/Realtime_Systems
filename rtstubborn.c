/***

stubborn.c      R.Tervo     Oct 2011
-------------------------------------
This program runs and stubbornly
refuses to stop as ^C ^Z ^\ keys
are trapped by signal handlers.
-------------------------------------
If another terminal window is opened
the above signals can be sent to the
PID using %kill -N PID and the same
a same stubborn responses observed.
%kill PID (default)sends SIGTERM
%kill -TERM PID    sends SIGTERM
%kill -15   PID    sends SIGTERM
%kill -l     lists signal names
-------------------------------------
Of course %kill -9 PID does the job
as we cannot overide SIGKILL
-------------------------------------
***/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void myhandler(int signum); // prototype
int i=24;
main(int argc, char *argv[])
{
int pid;
time_t now;

signal(SIGHUP , myhandler); // HangUp = 1
signal(SIGINT , myhandler); // CNTL-C = 2
signal(SIGQUIT, myhandler); // CNTL-\ = 3
signal(SIGALRM, myhandler); // Alarm  = 14
signal(SIGTERM, myhandler); // %kill  = 15
signal(SIGTSTP, myhandler); // CNTL-Z = 24

if( argc == 1 ) {
 pid = getpid();
 printf("Hello, my pid is [%i]\n", pid );
 printf("I am going to wait here for 60 seconds.\n");
 printf("See if you can stop me.\n");
 //alarm(1);
 myhandler(SIGHUP);
 time(&now);
 while( time('\0') < now + 60 );
 printf("I am done now, and you did not stop me.\n\n");
}
else
{
 printf("usage: stubborn  \n");
}

} // end main


void myhandler(int signum)
{
 printf("Signal [%i] received: ", signum);
 if( signum == SIGHUP  ) printf("Don't hang up on me!\n");
 if( signum == SIGTERM ) printf("I am not dead yet!\n");
 if( signum == SIGQUIT ) printf("Cntl-\\ does not work!\n");
 if( signum == SIGINT  ) printf("Cntl-C does not work!\n");
 if( signum == SIGTSTP ) printf("Cntl-Z does not work!\n");
 if( signum == SIGALRM ) printf("ALARM signal ok \n");
 signal( signum, myhandler );
}
