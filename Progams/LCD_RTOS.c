#include "freeRTOS.h"
#include "task.h"
#include "semphr.h"
SemaphoreHandle_t m;

void cmd(char a);
void data(char a);
void lcd1(void *a);
void lcd2(void *a);
void display(const char *);


 
int main(){
  m=xSemaphoreCreateMutex();

  PINSEL0=0;
  PINSEL1=0;	     				 //for turning every pins into GPIO mode
  PINSEL2=0;
  IODIR0=0xFFFFFFFF;  				//port0 as output
  cmd(0x38);
  cmd(0x0F);
  cmd(0x01);

  xTaskCreate(lcd1,"hello",90,NULL,0,NULL);
  xTaskCreate(lcd2,"welcome",90,NULL,0,NULL);
  vTaskStartScheduler();
  while(1);
}

 void cmd(char a){
    int i;
	IOCLR0=0x000003FF;				//first clear all
	IOSET0=a << 2;

	IOSET0=0x00000002;     			//RS=0 & E=1
	for(i=0;i<=100000;i++);
	IOCLR0=0x00000002;

 }
 void data(char a){
    int i;
    IOCLR0=0x000003FF;				//first clear all
	IOSET0=a << 2;

	IOSET0=0x00000003;   			//RS=1 & E=1
	for(i=0;i<=100000;i++);
	IOCLR0=0x00000002;			    //RS=1 & E=0

 }
 void display(const char *str){
	while(*str!='\0'){
	   data(*str);
	   str++;
	}

 }

void lcd1(void *a)
{

	while(1)
	{
		
		if(xSemaphoreTake(m,1000))
		{
		cmd(0x80);
		display("Hello");
		xSemaphoreGive(m);
		vTaskDelay(200);
		}
		
		
	}
}
void lcd2(void *a)
{
	while(1)
	{
		if(xSemaphoreTake(m,1000))
		{
		cmd(0xC0);
		display("Welcome");
		xSemaphoreGive(m);
		vTaskDelay(200);
		}
	}
}