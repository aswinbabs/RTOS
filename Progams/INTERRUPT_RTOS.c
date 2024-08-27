/* Standard includes. */
#include <stdlib.h>
#include<lpc213x.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

SemaphoreHandle_t xBinarySemaphore;
//#define mainINTERRUPT_NUMBER 3


static void vHandlerTask( void *pvParameters );

void blink1(void *pvParameters)
{
    while(1) 
		{
			IOCLR1=0x10000;//p1.16
			vTaskDelay(500);
			IOSET1=0x10000;
			vTaskDelay(500);
    			   
    }
}

void blink2(void *pvParameters)
{
    while(1) 
		{
		 	IOCLR1=0x20000;//p0.17
			vTaskDelay(1000);
			IOSET1=0x20000;
			vTaskDelay(500);
    }
}

void blink3(void *pvParameters)
{
    while(1) 
		{
			IOCLR1=0x40000;//p0.18
			vTaskDelay(1500);
			IOSET1=0x40000;
			vTaskDelay(500);			   
    }
}
void trans(char);
void uart0(void *pvParameters)
{
    while(1) 
		{
			trans('A');//A
			trans('B');//B
			vTaskDelay(1000);
		}
		
}

///////////////////////////////////////////

 void vIntrHandler( void ) __irq
{
	BaseType_t xHigherPriorityTaskWoken;
	vTaskSuspendAll();
	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
	trans('C');
	EXTINT=0x02;	   //EINT1
	xTaskResumeAll();
	portEXIT_SWITCHING_ISR( xHigherPriorityTaskWoken );
	VICVectAddr=0x00000000;
}
int main( void )
{
	IO0DIR=0x00000000;
	IO1DIR=0xFFFFFFFF;
	PINSEL0=0x20000001;							  //UART0,EINT1
	VICIntEnable=0x00008000;					  //EINT1
	VICVectCntl2=0x2F;							  //IRQ ENABLED,EINT1 MASK NO:15 OR YOU CAN WRITE VICVectCntl2=(1<<5)|15;
	VICVectAddr2=(unsigned)vIntrHandler;
	EXTMODE=0x02;
	EXTPOLAR=0x00;
	EXTINT=0x02;



U0LCR=0X83;
U0DLL=98;  //9600baud, 10Mhz
U0DLM=0;  //9600baud, 10Mhz
U0LCR=0X03;
trans('S');

xBinarySemaphore = xSemaphoreCreateBinary();
//portENABLE_INTERRUPTS();

xTaskCreate(blink1, (const char *)"Blink1", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
xTaskCreate(blink2, (const char *)"Blink2", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
xTaskCreate(blink3, (const char *)"Blink3", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
xTaskCreate(uart0,  (const char *)"uart0",  configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
xTaskCreate( vHandlerTask, "Handler", 1000, NULL, 3, NULL );
//vPortSetInterruptHandler( mainINTERRUPT_NUMBER, ulIntrHandler );
 
vTaskStartScheduler();

/* Should never reach here!  If you do then there was not enough heap
available for the idle task to be created. */
for( ;; );
}

static void vHandlerTask( void *pvParameters )
{
   for(;;)
   {
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
		IO1SET=0x00F00000;
		vTaskDelay(100);
		IO1CLR=0x00F00000;
		trans('D');	
   }

}

void trans(char b)
{
U0THR=b;
while((U0LSR&0X20)==0);
}