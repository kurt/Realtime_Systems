//***********************
//     3.2  SWITCH.C
/*
 * Measure overhead of using the sched_yield() call.
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sched.h>

int nswitch = 0;
pid_t chpid;
pid_t chpid2;
pid_t chpid3;

main()
{
        struct sigaction sa; // create a structure sa
        extern void alarm_handler(), child_terminate();

        sigemptyset(&sa.sa_mask); //sigemptyset is from signal I assume
        sa.sa_flags = 0; // initializing sa_flags to zero in sa structure
        sa.sa_handler = alarm_handler; // initializing sa_handler to be alarm_handler

		// if the signal action for the alarm
        if (sigaction(SIGALRM, &sa, NULL) < 0) {
                perror("sigaction SIGALARM");
                exit(1);
        }

		// set the handler in sa to the function child_terminate
        sa.sa_handler = child_terminate;

        if (sigaction(SIGUSR2, &sa, NULL) < 0) {
                perror("sigaction SIGUSR2");
                exit(1);
        }

        /* Chould set scheduler here */

        switch(chpid = fork()) {
                case -1:        /* Error */
                        perror("fork");
                        exit(3);
                        break;
                default:        /* Parent: set alarm and fall through to common case */
                        alarm(60);
                case 0:         /* Everybody */
                        switcher();
                        exit(0);
				switch(chpid2 = fork()) {
					case -1:        /* Error */
							perror("fork");
							exit(3);
							break;
					default:        /* Parent: set alarm and fall through to common case */
							alarm(60);
					case 0:         /* Everybody */
							switcher();
							exit(0);
							switch(chpid3 = fork()) {
								case -1:        /* Error */
										perror("fork");
										exit(3);
										break;
								default:        /* Parent: set alarm and fall through to common case */
										alarm(60);
								case 0:         /* Everybody */
										switcher();
										exit(0);
										break;
							} // end of switch 3
				}//end of switch 2
        } // end of switch 1

        printf("Unexpected exit from test program!\n");
        exit(4);
}
switcher()
{
        while(1) {
                sched_yield();
                nswitch++;
        }
}

void child_terminate()
{
        printf("%d switches in 60 seconds = %d switches/sec (child_term)\n",
                nswitch, nswitch/60);
        exit(0);
}

void alarm_handler()
{
        printf("%d switches calls in 60 seconds = %d switches/sec (alarm_han)\n",
                nswitch, nswitch/60);
        kill(chpid, SIGUSR2);
        exit(0);
}
