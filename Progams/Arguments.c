#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <lpc213x.h>
xQueueHandle q;
void lcd1(void *a);
void display(const char *);
void trans(char a);
int main()
{
	PINSEL0=1|1<<2;
	U0LCR=0x83;
	U0DLL=98;
	U0DLM=0;
	U0FDR=1<<4;
	U0LCR=0x03;
	PINSEL1=0;
	IO0DIR=0;
	IO1DIR=~0;
	trans('a');
	xTaskCreate(lcd1,"Welcome",90,(int *)120,0,NULL);
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
void lcd1(void *a)
{
	int b=(int )a;
	while(1)
	{
		if((IO0PIN&(1<<12))==(1<<12))
		{	
		trans(b/100+48);
			trans((b%100)/10+48);
			trans((b%10)+48);
		while((IO0PIN&(1<<12))==(1<<12));	
		}
	
		
	}
	}