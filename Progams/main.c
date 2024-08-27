#include "FreeRTOS.h"
#include "task.h"

void task1(void *); //declaration
void task2(void *); 

int main()
{
	PINSEL0=0;
	IO0DIR=0xc;
	xTaskCreate(task1,"t1",90,NULL,0,NULL);
	xTaskCreate(task2,"t2",90,NULL,0,NULL);
	vTaskStartScheduler();
	while(1);
}
/*(pvTaskCode)pointer to function,(pcName)for debugging,(usStackDepth)lengthof stack in words.The actual size of the stack depends on the
micro controller. If stack with is 32 bits (4 bytes) and usStackDepth is 100, then 400 bytes (4 times 100) will be
allocated for the task ,(pvParameters)a pointer to arguments given to the task,(uxPriority)priority given to the task, a number between 0 and MAX_PRIORITIES – 1, (pxCreatedTask) a pointer to an identifier that allows to handle the task*/
/*
portBASE_TYPE xTaskCreate( pdTASK_CODE pvTaskCode,  
                           const signed portCHAR * const pcName,  
                           unsigned portSHORT usStackDepth,  
                           void *pvParameters,  
                           unsigned portBASE_TYPE uxPriority,  
                           xTaskHandle *pxCreatedTask  
                         ); 
*/	

void task1(void *a)
{
	int i;
while(1)
{
	IO0SET=0x4;//p0.2
	vTaskDelay(100);
	
	IO0CLR=0x4;												     
	vTaskDelay(100);
}
}
void task2(void *a)
{
	int i;
while(1)
{
	IO0SET=0x8;//p0.3
	vTaskDelay(100);
	
	IO0CLR=0x8;
	vTaskDelay(100);
	
}
}