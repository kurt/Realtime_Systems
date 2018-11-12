//**********************************************************
#include <stdio.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>

int value=0;
int irq=0;

const stuct sigevent * handler(void *area. int id){
	value=~value;
	return(NULL); 
}


int main() {
	printf("Welcome to the program!");
	struct sigevent event;
	SIGEV_INTR_INIT(&event);
	ThreadCtl(_NTO_TCTL_IO, NULL);
	int id = InterruptAttach(irq,&handler,NULL,0,0);
	sleep(100);
	printf("Work Complete")
	return 0;
	/*
	while(1){
		InterruptWait(0, NULL);
		InterruptUnmask(irq,id);
		value=~value;
		delay(1);
		printf("%d", value);
	}
*/

	return 0;
}
//*******************************************************
