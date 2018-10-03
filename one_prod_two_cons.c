/*
scriptname: one_prod_two_cons.c
author: Richard Tervo
purpose: to be a part of lab
description: todoloo

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/neutrino.h>
#define MSGSIZE 80
int chid;
int rcvid;
unsigned flags;
char send_message[MSGSIZE]; 		//send message
char receive_message[MSGSIZE]; 		//receive message
char reader[MSGSIZE]; //
char readerreply[MSGSIZE];

// Thread Definitions
void * user()
{
	// Sends the data from command line to the friend.
	int ucoid;
	puts("user has been called");
	puts("user connecting to channel. \n");
	ucoid = ConnectAttach(0, 0, chid, 0, NULL);
	while(1) {
		puts("user:go ahead, enter a string");
		gets(send_message);
		printf("user:waiting to send: = %s =.\n",send_message);
		MsgSend(ucoid, &send_message, sizeof(send_message), &receive_message, sizeof(receive_message));
		printf("user: received a reply from friend: = %s =\n", receive_message);
		sleep(1);
	}
}

void * friend()
{
	int fcoid;
	puts("friend has been called");
	puts("friend connecting to channel. \n");
	fcoid = ConnectAttach(0, 0, chid, 0, NULL);
	strcpy(readerreply, "Got it!");
	while(1) {
		printf("friend: I am waiting to receive message from user.\n");
		rcvid = MsgReceive(chid, &reader, sizeof(reader), NULL);
		printf("friend: received = %s = from user.\n", reader);
		MsgReply(rcvid, NULL, &readerreply, sizeof(readerreply));
		sleep(1);
	}
}

void * add_cons()
{
	int cons_coid;
	puts("consumer has been called");
	puts("consumer connecting to channel. \n");
	cons_coid = ConnectAttach(0, 0, chid, 0, NULL);
	strcpy(readerreply, "Got it!");
	while(1) {
		printf("consumer: I am waiting to receive message from user.\n");
		rcvid = MsgReceive(chid, &reader, sizeof(reader), NULL);
		printf("consumer: received = %s = from user.\n", reader);
		MsgReply(rcvid, NULL, &readerreply, sizeof(readerreply));
		sleep(1);
	}
}

/*
void * add_producer()
	{
		int prod_coid;
		char temp[MSGSIZE];
		temp[0] =  'D';//(char[MSGSIZE])
		puts("Producer has been called");
		puts("Producer connecting to channel");
		prod_coid=ConnectAttach(0,0,chid,0,NULL);
		while(1){
			printf("user wasnts to send %s",temp);
			MsgSend(prod_coid, &temp, sizeof(temp), &receive_message, sizeof(receive_message));
			printf("producer: reply from friend %s", receive_message);
		}
	}
*/

// MAIN______________________________________________________________
int main(void)
{
	puts("Embedded Message Passing Example Program");
	puts("creating channel");
	flags = NULL;
	chid = ChannelCreate(flags);
	if (chid==1){
		printf("could not create channel \n");
		//exit(1);
	}
	pthread_create(NULL, NULL, &user,NULL);
	pthread_create(NULL, NULL, &friend,NULL);
	pthread_create(NULL,NULL, &add_cons,NULL);
	sleep(120);
	return(0);
}


