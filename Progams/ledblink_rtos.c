#include "FreeRTOS.h"
#include "task.h"

void task1(void *); //declaration
void task2(void *);
void task3(void *); 

int main(){
	PINSEL0=0;
	PINSEL2=0;
	IODIR0=0x8;			//S/W 
	IODIR1=0x04440000;  //LEDS
	xTaskCreate(task1,"t1",90,NULL,0,NULL);
	xTaskCreate(task2,"t2",90,NULL,0,NULL);
	xTaskCreate(task3,"t3",90,NULL,0,NULL);
	vTaskStartScheduler();
	while(1);
}

void task1(void *a){
  while(1){
    if((IOPIN0&0x1)==0){
	  IOSET1=0x00040000; //led1
	  vTaskDelay(100);		//delay for the same to complete within 100 ms
	  }
	else{
	 IOCLR1=0x00040000; 

	 }
	}
}
void task2(void *b){
 while(1){
  if((IOPIN0&0x2)==0){
   IOSET1=0x00400000; //led2
   vTaskDelay(100);
  }
  else{
   IOCLR1=0x00400000;
 
  }
 }
}
void task3(void *c){
 while(1){
  if((IOPIN0&0x4)==0){
   IOSET1=0x04000000; //led3
   vTaskDelay(100);
  }
  else{
   IOCLR1=0x04000000;

  }
 }
}
