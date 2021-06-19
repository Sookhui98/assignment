#include "stm32f10x.h"
#include "cmsis_os.h"
#include "uart.h"


osMutexId uart_mutex;
osMutexDef(uart_mutex);

const unsigned int size = 5;

unsigned char buffer[size];
void producer (void const *argument);																
void consumer (void const *argument);

unsigned int writepointer = 0;
unsigned int readpointer = 0;
unsigned int variable = 0x30;
//unsigned int variable = 0x41;  Testing code, the variable become alphabet instead of integer
unsigned int data = 0x00;


osThreadDef(producer, osPriorityNormal, 1, 0);
osThreadDef(consumer, osPriorityNormal, 1, 0);

osSemaphoreId inserted_item;									
osSemaphoreDef(inserted_item);
osSemaphoreId space;									
osSemaphoreDef(space);
osThreadId T_uart1; //producer
osThreadId T_uart2; //consumer 

void producer (void const *argument) 
{
	for (;;) 
	{
		osSemaphoreWait(space, osWaitForever);  
		osMutexWait(uart_mutex, osWaitForever);
		buffer[writepointer]=variable;
		variable++;
   		writepointer=(writepointer+1)%size;		
		osMutexRelease(uart_mutex);
		osSemaphoreRelease(inserted_item); //item inserted signal
	  if (variable == 0x3A){
				variable = 0x30;
				}      
	   }
  }

void consumer (void const *argument) 
{
	for (;;) 
	{
		osSemaphoreWait(inserted_item, osWaitForever);
		osMutexWait(uart_mutex, osWaitForever);
		data=buffer[readpointer];	
		readpointer=(readpointer+1)%size;		
		osMutexRelease(uart_mutex);
		osSemaphoreRelease(space);
		SendChar(data);
		if ((readpointer%5) == 0){
			SendChar('\n');
			}
		
	}

}

int main (void) 
{
	osKernelInitialize (); 	// initialize CMSIS-RTOS
	
	USART1_Init ();
	uart_mutex = osMutexCreate(osMutex(uart_mutex));					//create the message queue
	T_uart1 = osThreadCreate(osThread(producer), NULL);
	T_uart2 = osThreadCreate(osThread(consumer), NULL);
	inserted_item = osSemaphoreCreate(osSemaphore(inserted_item), 0);	
	space = osSemaphoreCreate(osSemaphore(space), size);
	osKernelStart ();  
}
