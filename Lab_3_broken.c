/* code name: Lab_3_broken.c
 * author: Kurt Stewart
 *
 * Purpose: working with semaphores to create mutex
 *
 * Description:
 *
 * Left off: I got it printing although I think that there should be stuff changed (going to go back to original design)
 *
 * To do:
 */

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>


// Global Variables
int data_buffer[200];
//int count=0;
//int put_index=0;
//int get_index=0;

// Semaphore Definitions
sem_t sem_access_buffer;
sem_t sem_data_in_buffer;

// Function Definitions
void checkresult( int result, char *text );

void init_buff(){
	int i;
	for(i=0;i<8;i++){
		data_buffer[i]=14;
	}
	return;
}

void put_item(){
	// puts items into buffer
	//sem_wait(&sem_access_buffer);
	int i;
	while (1){
		sem_wait(&sem_access_buffer);
		data_buffer[i]=i;
		sem_post(&sem_access_buffer);
		sem_post(&sem_data_in_buffer);
		put_index++;
		return;
	}
}

void get_item(){
	int data, i;
	while(1){
		// wait for the first data to be made
		sem_wait(&sem_data_in_buffer);
		//sem_post(&sem_data_in_buffer);
		sem_wait(&sem_access_buffer);
		data=data_buffer[i];
		data_buffer[i]=0;
		sem_post(&sem_access_buffer);
		get_index++;
	}
	//sem_post(sem_put_caught_up);
}


//___________________________________________________________________
// Main
//___________________________________________________________________
int main(int argc, char *argv[]) {
	int result, p, data;

	printf("Hello This is Lab_3.c\n\r");
	fflush(stdout);
	
	// semaphore inits
	sem_init(&sem_access_buffer,0,1);
	sem_init(&sem_data_in_buffer,0,1);
	
	//Initialize Buffer
	init_buff();

	//start threads
	struct sched_param get_sched;
	struct sched_param put_sched;

	pthread_t put_item_thread;
	pthread_t get_item_thread;

	result=pthread_create(&get_item_thread, NULL, get_item, NULL);
	checkresult(result,"get thread fail");
	result=pthread_create(&put_item_thread, NULL, put_item, NULL);
	checkresult(result,"put thread fail");

	for(p=0; p<200; p++ )
	 { sleep(1);
	 //data=get_item();
	   //printf("data: %d\n\r", data_buffer[0]);
	   //printf("countget: %d\n\r",get_index);
	   //printf("countput: %d\n\r",put_index);
	 }

	exit(0); // never exit?
}// end of main


void checkresult( int result, char *text )
{
if( result == -1 )
{
 perror(text);
 exit(1);
}
}
