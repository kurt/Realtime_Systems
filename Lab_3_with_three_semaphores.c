/* code name: Lab_3.c
 * author: Kurt Stewart
 *
 * Purpose: working with semaphores to create mutex
 *
 * Description: three-way semaphores to make sure threads do not get data not yet put or put data over data not yet gotten
 *
 * Left off: Have not got the circular index or (%) for put_caught_up semaphore
 *
 * To do:
 */

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>


// Global Variables
int data_buffer[8];
int get_index=0, put_index=0;

// Semaphore Definitions
sem_t sem_access_buffer;
sem_t sem_get_caught_up;
sem_t sem_put_caught_up;

// Function Definitions
void checkresult( int result, char *text );

void init_buff(){
	int i;
	for(i=0;i<8;i++){
		data_buffer[i]=0;
	}
}

void put_item(){
	sem_wait(&sem_put_caught_up);
	sem_wait(&sem_access_buffer);
	data_buffer[put_index%8]=put_index;
	put_index++;
	sem_post(&sem_access_buffer);
	sem_post(&sem_get_caught_up);
}

int get_item(){
	int data;
	sem_wait(&sem_get_caught_up);
	sem_wait(&sem_access_buffer);
	data=data_buffer[get_index];
	get_index++;
	sem_post(&sem_access_buffer);
	sem_post(&sem_put_caught_up);
	return data;
}


//___________________________________________________________________
// Main
//___________________________________________________________________
int main(int argc, char *argv[]) {
	int result;

	printf("Hello This is Lab_3.c\n\r");
	// semaphore inits
	sem_init(sem_access_buffer,0,1); // ensures that only one function is accessing the buffer at once
	sem_init(sem_get_caught_up,0,0);
	sem_init(sem_put_caught_up,0,1); //will allow put to fill the buffer before getting from it
	//call init_buff
	init_buff();

	//start threads
	pthread_t put_item_thread;
	pthread_t get_item_thread;
	result=pthread_create(&put_item_thread, NULL, put_item, NULL);
	checkresult(result,"put thread fail");
	result=pthread_create(&get_item_thread, NULL, get_item, NULL);
	checkresult(result,"get thread fail");

	for(p=0; p<200; p++ )
	 { wait(1);
		data=get_item();
		printf("%d", data);
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
