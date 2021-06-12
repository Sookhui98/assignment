#include "stm32f10x.h"
#include "cmsis_os.h"
#include "uart.h"
#include "Board_LED.h"

osMessageQId Q_LED;																		//define the message queue
osMessageQDef (Q_LED,0x16,unsigned char);
osEvent  result;
osMutexId uart_mutex;
osMutexDef(uart_mutex);

void producer (void const *argument);																
void consumer (void const *argument);
int BUFFER_SIZE [4];
int bufferlength = 0;
int readpointer = 0;
int writepointer = 0;
int j;
int i;
osThreadDef(producer, osPriorityNormal, 1, 0);
osThreadDef(consumer, osPriorityAboveNormal, 1, 0);
osSemaphoreId sem1;									
osSemaphoreDef(sem1);
osThreadId T_uart1; //producer
osThreadId T_uart2; //consumer
osThreadId	T_main; 


void producer (void const *argument) 
{
	for (;;) 
	{
		osMutexWait(uart_mutex, osWaitForever);
    for(i=0;i<5;++i){
			for (j=0;j<4;++j){
			  SendChar('1');
		}
		osMutexRelease(uart_mutex);
		osSemaphoreRelease(sem1);
	   }
  }
}

void consumer (void const *argument) 
{
	for (;;) 
	{
		osSemaphoreWait(sem1, osWaitForever);
		osMutexWait(uart_mutex, osWaitForever);
		for (i=5; i>=0; --i){
			for (j=0; j<=i; ++j){
				SendChar('0');
		osMutexRelease(uart_mutex);
	}
 }
}
	}
int main (void) 
{
	osKernelInitialize (); 	// initialize CMSIS-RTOS
	
	USART1_Init ();
	uart_mutex = osMutexCreate(osMutex(uart_mutex));					//create the message queue
	T_uart1 = osThreadCreate(osThread(producer), NULL);
	T_uart2 =	osThreadCreate(osThread(consumer), NULL);
	
	osKernelStart ();  
}

