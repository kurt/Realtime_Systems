/* NOSWITCH.C
 * Measure overhead of using the sched_yield() call.
 
 There is a structure that contains the handler for the alarm. 
 
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sched.h>

int nyield = 0;

main()
{
        struct sigaction sa;
        extern void alarm_handler();

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = alarm_handler;

        if (sigaction(SIGALRM, &sa, NULL) < 0) {
                perror("sigaction SIGALARM");
                exit(1);
        }
        switcher(); /* Switch with self - i.e., no context switch */
        printf("Unexpected exit from test program!\n");
        exit(4);
}

switcher()
{
	// sets a one shot alarm to go off in 60 seconds and then continues to do sched_yield
        alarm(60);
        while(1) {
                sched_yield();
                nyield++;
        }
}

void alarm_handler()
{
        printf("%d yield calls in 60 seconds = %d yield/sec\n",
                nyield, nyield/60);
        exit(0);
}
