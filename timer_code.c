/*
scriptname: timer_code.c 
author: Kurt Stewart
purpose: assignment 3 pt2 
description: 
	adapted from example code from timer_create() qnx documentation the following code has two threads
	that wait on monotonic timer interrupts 

*/



#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL
#define Other			_PULSE_CODE_MAXAVAIL

static void *myfunction( void *args );
static void *myfunction2( void *args );

typedef union {
        struct _pulse   pulse;
        /* your other message structures would go
           here too */
} my_message_t;

int                     chid;		//chan
int                     rcvid;	// receive id
my_message_t            msg;		// message object

int                     chid2;		//chan
int                     rcvid2;	// receive id
my_message_t            msg2;		// message object


main()
{
   struct sigevent         event;		//create signal event struct
   struct itimerspec       itime;     //create timer struct
   timer_t                 timer_id;	//timer object


   int result;
   // Create Channel
   chid = ChannelCreate(0);
   SIGEV_INTR_INIT(&event);	//Add interrupt ability to the signal event
   InterruptAttachEvent(10,&event,0); // attach the IRQ10 with the signal event interrupt with no flag
   //event.sigev_intr_init()
   event.sigev_notify = SIGEV_PULSE;	//the notification of the signal event is pulse type
   event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,chid,_NTO_SIDE_CHANNEL, 0); //the channel on which the signal will be listening is the created channel
   event.sigev_priority = getprio(0);	//priority of 0
   event.sigev_code = MY_PULSE_CODE;	//the code that executes upon the signal event is MY_PULSE_CODE
   timer_create(CLOCK_MONOTONIC, &event, &timer_id);	//create a monotonic timer which signals the event of timerid

   itime.it_value.tv_sec = 1;
   /* 500 million nsecs = .5 secs */
   itime.it_value.tv_nsec = 500000000;
   itime.it_interval.tv_sec = 10;
   /* 500 million nsecs = .5 secs */
   itime.it_interval.tv_nsec = 500000000;
   timer_settime(timer_id, 0, &itime, NULL);


   pthread_t tid;
   result = pthread_create( &tid, NULL, myfunction, NULL );
   //pthread_setschedprio( tid,9 );
   //----------------------------------------------------------------------------------------------------
   	 struct sigevent         event2;		//create signal event struct
     struct itimerspec       itime2;     //create timer struct
     timer_t                 timer_id2;	//timer object


     int result2;
     // Create Channel
     chid2 = ChannelCreate(0);
     SIGEV_INTR_INIT(&event2);	//Add interrupt ability to the signal event
     InterruptAttachEvent(9,&event2,0); // attach the IRQ10 with the signal event interrupt with no flag
     //event.sigev_intr_init()
     event2.sigev_notify = SIGEV_PULSE;	//the notification of the signal event is pulse type
     event2.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,chid2,_NTO_SIDE_CHANNEL, 0); //the channel on which the signal will be listening is the created channel
     event2.sigev_priority = getprio(0);	//priority of 0
     event2.sigev_code = MY_PULSE_CODE;	//the code that executes upon the signal event is MY_PULSE_CODE
     timer_create(CLOCK_MONOTONIC, &event2, &timer_id2);	//create a monotonic timer which signals the event of timerid

     itime2.it_value.tv_sec = 1;
     /* 500 million nsecs = .5 secs */
     itime2.it_value.tv_nsec = 500000000;
     itime2.it_interval.tv_sec = 1;
     /* 500 million nsecs = .5 secs */
     itime2.it_interval.tv_nsec = 500000000;
     timer_settime(timer_id2, 0, &itime2, NULL);
     pthread_t tid2;

     result2 = pthread_create( &tid2, NULL, myfunction2, NULL );
     //pthread_setschedprio( tid2,9 );

   //loop forever
    while(1){}
}//end of main

static void *myfunction( void *args ){
	while(1){
	rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL); //wait for message on the channel
	if (rcvid == 0) { /* we got a pulse */
            if (msg.pulse.code == MY_PULSE_CODE) {
                printf("we got a pulse from our timer\n");
            } /* else other pulses ... */
       } /* else other messages ... */
	}
}

static void *myfunction2( void *args ){
	while(1){
	rcvid2 = MsgReceive(chid2, &msg2, sizeof(msg2), NULL); //wait for message on the channel
	if (rcvid2 == 0) { /* we got a pulse */
            if (msg2.pulse.code == MY_PULSE_CODE) {
                printf("we got a pulse from our second timer\n");
            } /* else other pulses ... */
       } /* else other messages ... */
	}
}

