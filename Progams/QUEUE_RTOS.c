#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <lpc213x.h>
void display(const char *);
void trans(char a);
SemaphoreHandle_t m;
xQueueHandle q;

void task1(void *); //declaration
void task2(void *); 

int main()
{m=xSemaphoreCreateMutex();
	q=xQueueCreate(5,4); //char a[5] sizeof(int)
	PINSEL0=1|1<<2;//uart
	U0LCR=0x83;
	U0DLL=98;
	U0DLM=0;
	U0FDR=1<<4;
	U0LCR=0x03;//uart
	PINSEL1=0;
	IO0DIR=0;
	IO1DIR=~0;
	xTaskCreate(task1,"t1",90,NULL,1,NULL);
	xTaskCreate(task2,"t2",90,NULL,1,NULL);
	vTaskStartScheduler();
	while(1);
}
	
void trans(char a)
{
while((U0LSR&(1<<5))==0);				//U0THR EMPTY
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
	int i=8;
while(1)
{

if((IO0PIN&(1<<12))==(1<<12)) //pullup pulldown
		{
		i++;
		display("\rSending to queue\r");
		if(xQueueSend(q,&i,1000))			   //'q' returned by queue create, 'i' pointer to the element to be copied, delay
		{
			if(xSemaphoreTake(m,3000)==1)
			{
			display("\rSuccess\r");
				xSemaphoreGive(m);	
			}
		}
		else
		{
					if(xSemaphoreTake(m,3000)==1)
			{
			display("\rFailed to write\r");
					xSemaphoreGive(m);	
			}
		}
		while((IO0PIN&(1<<12))==(1<<12));
	
	}
}
}
void task2(void *a)
{
	int r;
	char num[5];
	signed char j;//unsigned format
while(1)
{
	if((IO0PIN&(1<<11))==(1<<11))
	{
		if(xSemaphoreTake(m,3000)==1)
			{
			display("\rReading from queue\r");
			xSemaphoreGive(m);	
			}
		if(xQueueReceive(q,&r,100))				 // 'q' is the identifier of the queue to be read ,'&r'is a pointer to the buffer where the read value will be copied to , delay
		{
					if(xSemaphoreTake(m,3000)==1)
			{
			display("\rSuccessfully read\r");
						j=0;
			do
				{
				num[j]=r%10;//integer data type 8 bit size
				r=r/10;//
				j++;
				}while(r!=0);
			for(j--;j>=0;j--)//
				{
				trans(num[j]+48);
				}
				xSemaphoreGive(m);	
			}
		}
		else
		{
				if(xSemaphoreTake(m,3000)==1)
			{
			display("\rFailed  to read, No data\r");
								xSemaphoreGive(m);
			}
		}
		while((IO0PIN&(1<<11))==(1<<11));
	}
	
}
}