/*
Scriptname: assign_1.c

Author: Kurt Stewart
*/

#include<stdio.h>

void main(){
int array[50], value=23, new_val,i; 
for(i=0;i<50;i++){
	array[i]=i;
}// end of for
array[1]=value;
printf("array is: %d\n\r",array[1]);
new_val=array[30];
printf("new_val is: %d\n\r", new_val);
}
