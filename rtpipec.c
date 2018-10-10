#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/***
------------------------------------------
pipe.c          R.Tervo          Jan 2013
------------------------------------------
***/

main ()
{

char strA[100], strB[100];

pid_t pidC;

int fd[3];
int result, status;

strcpy(strA, "------------"); // empty string



result = pipe(fd);
if( result == -1 )
{ // error has occured
  perror("failed pipe()");
  exit(1);
} // end error

printf("FILE DESCRIPTORS: fd[0]=%i, fd[1]=%i \n\n", fd[0], fd[1] );

result = fork();
if( result == -1 )
{ // error has occured
  perror("failed fork()");
  exit(1);
} // end error

pidC = result;  // record result of pipe

if( pidC == 0 ) // identify child process
{
  close(fd[0]);
 printf("child  PID= %i \n", getpid() );

 strcpy(strA,"Hello"); // 6 bytes
 printf(" child sends [%s]\n", strA );


  result = write(fd[1], strA, 6);
  if( result == -1 )
  { // error has occured
    perror("failed write()");
    exit(1);
  } // end error

} else 
{ close(fd[1]);

 printf("parent PID= %i \n", getpid() );
 
    read(fd[0], strA ,6);
 printf("parent reads [%s]\n", strA );

 result = wait(&status);
 printf("wait returns [%i] from pid [%i]\n", status, result);

}
} // end main