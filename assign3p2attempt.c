
#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>

#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL
#define MY_OTHER_PULSE_CODE _PULSE_CODE_MAXAVAIL

typedef union {
        struct _pulse   pulse;
        /* your other message structures would go
           here too */
} my_message_t;

main()
{
   struct sigevent         timer_event;
   struct sigevent         timer_event2;
   struct itimerspec       interval_time;
   struct itimerspec       interval_time2;
   timer_t                 timer_id;
   timer_t				   timer_id2;
   int                     chid, chid2;
   int                     rcvid, rcvid2;
   my_message_t            msg;




   //------------------First timer------------------------------
   chid = ChannelCreate(0);
   // connect the message channel
   timer_event.sigev_notify = SIGEV_PULSE;
   timer_event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,chid,_NTO_SIDE_CHANNEL, 0);
   timer_event.sigev_priority = getprio(0);
   timer_event.sigev_code = MY_PULSE_CODE;
   //create timer of type monotonic calling the event "timer_event"
   timer_create(CLOCK_MONOTONIC, &timer_event, &timer_id);

   // set the first shot to be 1 second
   interval_time.it_value.tv_sec = 1;
   /* 500 million nsecs = .5 secs */
   //interval_time.it_value.tv_nsec = 500000000;
   // set each interval to be 1 second
   interval_time.it_interval.tv_sec = 1;
   /* 500 million nsecs = .5 secs */
   //interval_time.it_interval.tv_nsec = 500000000;

   //update the timer to the loaded time
   timer_settime(timer_id, 0, &interval_time, NULL);

   //--------------------Second Timer--------------------------------------
  // chid2 = ChannelCreate(1);
   timer_event2.sigev_notify = SIGEV_PULSE;
   timer_event2.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,chid,_NTO_SIDE_CHANNEL, 0);
   timer_event2.sigev_priority = getprio(0);
   timer_event2.sigev_code = MY_OTHER_PULSE_CODE;
   //create timer of type monotonic calling the event "timer_event"
   timer_create(CLOCK_MONOTONIC, &timer_event2, &timer_id2);

     // set the second timer to be 2.5 seconds
     interval_time2.it_value.tv_sec = 2;
     /* 500 million nsecs = .5 secs */
     interval_time.it_value.tv_nsec = 500000000;
     // set each interval to be 1 second
     interval_time2.it_interval.tv_sec = 2;
     /* 500 million nsecs = .5 secs */
     interval_time.it_interval.tv_nsec = 500000000;

     //update the timer to the loaded time
     timer_settime(timer_id2, 0, &interval_time2, NULL);


int i;
   for (i=1;i<10;i++) {
	   //wait for a pulse on the channel
       rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
       if (rcvid == 0) { /* we got a pulse */
            if (msg.pulse.code == MY_PULSE_CODE) {
                printf("we got a pulse from our first timer\n");
            } /* else other pulses ... */
            else{
            	printf("we got a pulse from our second timer\n");
            }
       }
  /*
       rcvid2 = MsgReceive(chid2, &msg, sizeof(msg), NULL);
       if (rcvid2 == 0) { /* we got a pulse
                   if (msg.pulse.code == MY_PULSE_CODE) {
                       printf("we got a pulse from our second timer\n");
                   }
              }
              */
   }
}
