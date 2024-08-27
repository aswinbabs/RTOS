 #include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <lpc213x.h>
void display(const char *);
void trans(char a);

xQueueHandle q;

void task1(void *); //declaration
void task2(void *); 

int main()
{
	q=xQueueCreate(5,sizeof(char));
	PINSEL0=1|1<<2;
	U0LCR=0x83;
	U0DLL=98;
	U0DLM=0;
	U0FDR=1<<4;
	U0LCR=0x03;
	PINSEL1=0;
	IO0DIR=0;
	IO1DIR=~0;
	xTaskCreate(task1,"t1",90,(void *)'W',1,NULL);
	xTaskCreate(task2,"t2",90,NULL,1,NULL);
	vTaskStartScheduler();
	while(1);
}
	
void trans(char a)
{
while((U0LSR&(1<<5))==0);
U0THR=a;

}
void display(const char *a)
{
	while(*a)
	{
	trans(*a++);
	}
}
void task1(void *a)//send q
{
	char i=(char )a;		 //typecasting
    while(1){
       if((IO0PIN&(1<<12))==(1<<12)){

		  display("\rSending to queue\r");
		  if(xQueueSend(q,&i,1000)){
			display("\rSuccess\r");
			break;
		  }
		  else{
			display("\rFailed to write\r");
		  }
		  i++;
		while((IO0PIN&(1<<12))==(1<<12));
	
	   }
	
    }
vTaskDelete(NULL);			  //self task delete
}
void task2(void *a)
{
	char i;
    while(1)
     {
     	if((IO0PIN&(1<<11))==(1<<11))
	      {
			display("\rReading from queue\r");
		
		    if(xQueueReceive(q,&i,100))
		      {
			    display("\rSuccessfully read\r");
			    trans(i);
		      }
		    else
		      {
			    display("\rFailed  to read\r");
			  }
		    while((IO0PIN&(1<<11))==(1<<11));
	      }
	
 	 }
}