#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


SemaphoreHandle_t b;
void task1(void *); //declaration
void task2(void *); 

int main(){
    b=xSemaphoreCreateBinary();
	PINSEL0=0;

	IODIR1=0xff<<16;//output
	IODIR0=0;//Input
	xTaskCreate(task1,"t1",90,NULL,1,NULL);
	xTaskCreate(task2,"t2",90,NULL,1,NULL);
	vTaskStartScheduler();
	while(1);
}
void task1(void *a){
	int i;
    while(1){
     if(xSemaphoreTake(b,100)){
	   for(i=0;i<5;i++){
	     IOSET1=0xf<<16;
	     vTaskDelay(50);//1s
																      //take
	     IOCLR1=0xf<<16;
	     vTaskDelay(50);
	   }
     }
    IOSET1=0xf<<20;
	vTaskDelay(50);//1s
	
	IOCLR1=0xf<<20;
	vTaskDelay(50);
   }
}
void task2(void *a){													//give
	int i;
    while(1){
	 if((IOPIN0&(1<<12))==(1<<12)){
		xSemaphoreGive(b);
		while((IOPIN0&(1<<12))==(1<<12));
	 }
	
   }
}