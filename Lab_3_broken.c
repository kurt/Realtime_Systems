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
int data_buffer[8];
int count=0;
int put_index=0;
int get_index=0;

// Semaphore Definitions
sem_t sem_access_buffer;
sem_t sem_count;

// Function Definitions
void checkresult( int result, char *text );

void init_buff(){
	int i;
	for(i=0;i<8;i++){
		data_buffer[i]=14;
	}
}

void put_item(){
	sem_wait(&sem_count);
	count++;
	sem_post(&sem_count);
	sem_wait(&sem_access_buffer);
	data_buffer[put_index%8]=put_index;
	put_index++;
	sem_post(&sem_access_buffer);
	
}

int get_item(){
	int data;
	sem_wait(&sem_count);
	sem_wait(&sem_access_buffer);
	if (count > 0){
		data=data_buffer[get_index%8];
		get_index++;
		count--;
	}
	else{
		//sem_wait(&sem_count);
	}

	sem_post(&sem_access_buffer);
	
	return data;
	//sem_post(sem_put_caught_up);
}


//___________________________________________________________________
// Main
//___________________________________________________________________
int main(int argc, char *argv[]) {
	int result, p;

	printf("Hello This is Lab_3.c\n\r");
	fflush(stdout);
	// semaphore inits
	sem_init(&sem_access_buffer,0,1);
	sem_init(&sem_count,0,1);
	//call init_buff
	init_buff();

	//start threads
	pthread_t put_item_thread;
	pthread_t get_item_thread;

	result=pthread_create(&get_item_thread, NULL, get_item, NULL);
	checkresult(result,"get thread fail");
	result=pthread_create(&put_item_thread, NULL, put_item, NULL);
	checkresult(result,"put thread fail");

	for(p=0; p<200; p++ )
	 { wait(1);
	   printf("%d", data_buffer[1]);
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
