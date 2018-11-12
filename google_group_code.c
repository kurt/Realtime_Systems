void * WriterThread2(void* arg) 
{ 
        printf( "writer thread, thread id: %d\n", pthread_self() ); 

        /**initialize the timer*/ 


        timer_t writer_timer; 
        struct sigevent timer_event; 
        //SIGEV_INTR_INIT(&timer_event); 
        timer_event.sigev_notify = SIGEV_INTR;/** set the timer event to interrrupt*/ 

        struct itimerspec timer_spec;/**setting the timer to fire every 100ms*/ 
        timer_spec.it_interval.tv_sec = 0; 
        timer_spec.it_interval.tv_nsec = 500000000; 
        timer_spec.it_value.tv_sec = 0; 
        timer_spec.it_value.tv_nsec = 500000000; 
        // 
        if(0!=timer_create( CLOCK_REALTIME, &timer_event, &writer_timer))        /** create timer*/ 
        { 
                fprintf(stderr, "create timer error, at file: %s, function: %s, line: %d\n, error info: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno)); 
                return (0); 
        } 

        //ThreadCtl(_NTO_TCTL_IO, 0);/** before calling InterruptAttach, the thread must request I/O privilege,for timer interrupt, this should not be executed. why ?*/ 
        int interrupt_id = InterruptAttachEvent(0, &timer_event, 0);/**attach timer interrrupt event*/ 
//        if(-1==interrupt_id) 
//        { 
//                fprintf(stderr, "attach timer interrupt event error, at file: %s, function: %s, line: %d\n, error info: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno)); 
//                return (0); 
//        } 

        /** set and start the timer */ 
        if(0!=timer_settime(writer_timer, 0, &timer_spec, NULL))/** automatically unmask the timer interrrupt*/ 
        { 
                fprintf(stderr, "set and start timer error, at file: %s, function: %s, line: %d\n, error info: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno)); 
                return (0); 
        } 


        InterruptUnmask(0, interrupt_id);/**unmask the interrupt, this line could be omitted as the timer_settime function automatically unmask the timer interrupt*/ 

        struct timespec current_time; 
        while(1) 
        { 
                InterruptWait(0, NULL);/**wait for timer interrupt*/ 
                clock_gettime(CLOCK_REALTIME, &current_time); 
                fprintf(stdout, "timer fired, timestamp: %lf\n", current_time.tv_sec+current_time.tv_nsec/1000000000.0); 

                InterruptUnmask(0, interrupt_id);/**unmask the interrupt*/ 
        } 

        return( 0 ); 
} 

int main( void ) 
{ 

        printf("main thread, thread id: %d\n", pthread_self()); 

        pthread_t writer_thread; 
        if(EOK!=pthread_create( &writer_thread, NULL, &WriterThread2, NULL )) 
        { 
                fprintf(stderr, "write lock error, at file: %s, function: %s, line: %d\n, error info: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno)); 
                return (-1); 
        } 

        fprintf(stdout, "main thread waiting...\n"); 

        pthread_join(writer_thread, NULL); 
        printf("writer thread joined, main thread ending..."); 
        return EXIT_SUCCESS; 
} 