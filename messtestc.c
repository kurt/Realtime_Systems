/*
scriptname: mseetestc.c
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
char smsg[MSGSIZE];
char rmsg[MSGSIZE];
char rdr[MSGSIZE];
char rdrreply[MSGSIZE];


int main(void)
{
puts("Embedded Message Passing
Example Program");
puts("creating channel");
flags = NULL;
chid = ChannelCreate(flags);
if (chid == 1){
printf("could not create channel \n");
return(1);
}
pthread_create(NULL, NULL, &user,
NULL);
pthread_create(NULL, NULL, &friend,
NULL);
sleep(120);
return(0);
}


void * user()
{
int ucoid;
puts("user has been called");
puts("user connecting to channel. \n");
ucoid = ConnectAttach(0, 0, chid, 0, NULL);
while(1) {
puts("user:go ahead, enter a string");
gets(smsg);
printf("user:waiting to send: = %s =.\n",smsg);
MsgSend(ucoid, &smsg, sizeof(smsg), &rmsg, sizeof(rmsg));
printf("user: received a reply from friend: = %s =\n", rmsg);
sleep(1);
}
}

void * friend()
{
int fcoid;
puts("friend has been called");
puts("friend connecting to channel. \n");
fcoid = ConnectAttach(0, 0, chid, 0, NULL);
strcpy(rdrreply, "Got it!");
while(1) {
printf("friend: I am waiting to receive message from user.\n");
rcvid = MsgReceive(chid, &rdr, sizeof(rdr), NULL);
printf("friend: received = %s = from user.\n", rdr);
MsgReply(rcvid, NULL, &rdrreply, sizeof(rdrreply));
sleep(1);
}
}
