#include "stm32f10x.h"
#include "cmsis_os.h"
#include "uart.h"

osMutexId uart_mutex; 
osMutexDef(uart_mutex);
const unsigned int size = 5;
int data[5]={1,2,2,4,4};
int clear[5] ={0,0,0,0,0};
int buffer[size];
void producer (void const *argument);																
void consumer (void const *argument);
int bufferlength = 0;
int readpointer = 0;
int writepointer = 0;
int i;

osThreadDef(producer, osPriorityNormal, 1, 0);
osThreadDef(consumer, osPriorityAboveNormal, 1, 0);
osSemaphoreId sem1;									
osSemaphoreDef(sem1);
osThreadId T_uart1; //producer
osThreadId T_uart2; //consumer
osThreadId T_main; 


void producer (void const *argument) 
{
	for (;;) 
	{
		osMutexWait(uart_mutex, osWaitForever);
		for(i=0;i<5;i++){
		buffer[i]=data[i];
                writepointer = (writepointer+1) % size;	  //return writepointer to 0 after it reached 5		
	}

		osSemaphoreRelease(sem1);
	        osMutexRelease(uart_mutex);
	   }
  }

void consumer (void const *argument) 
{
	for (;;) 
	{
		osSemaphoreWait(sem1, osWaitForever);
		osMutexWait(uart_mutex, osWaitForever);
		for(i=0;i<5;i++){
		buffer[i]=clear[i];	
	        readpointer = (readpointer+1) % size;	  //return readpointer to 0 after it reached 5	
         }
		
		osMutexRelease(uart_mutex);
	}
 }

int main (void) 
{
	osKernelInitialize (); 	// initialize CMSIS-RTOS
	
	USART1_Init ();
	uart_mutex = osMutexCreate(osMutex(uart_mutex));	//create the message queue
	T_uart1 = osThreadCreate(osThread(producer), NULL);
	T_uart2 = osThreadCreate(osThread(consumer), NULL);
	sem1 = osSemaphoreCreate(osSemaphore(sem1), 0);	       //token initialised in sem1 = 0
	
	osKernelStart ();  
}
