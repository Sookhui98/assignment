#include "stm32f10x.h"
#include "cmsis_os.h"
#include "uart.h"
#include "Board_LED.h"

osMessageQId Q_LED;																		//define the message queue
osMessageQDef (Q_LED,0x16,unsigned char);
osEvent  result;
osMutexId uart_mutex;
osMutexDef(uart_mutex);
int size = 5;
int data[5]={1,2,2,4,4};
int clear[5] ={0,0,0,0,0};
int buffer[5];
void producer (void const *argument);																
void consumer (void const *argument);
int BUFFER_SIZE [4];
int bufferlength = 0;
int readpointer = 0;
int writepointer = 0;
int j,i;
int z=0;
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
		for(i=0;i<5;i++){
				buffer[i]=data[i];
        writepointer++;			
		}

 /*   for(i=1;i<=5;++i){
			for (j=1;j<=i;++j){
				buffer[z]=data[z];
			  SendChar('*');		
		}
			SendChar('\n');
	}*/
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
			  readpointer++;
        			
		}
	/*for (i=5; i>=1; --i){
			for (j=1; j<=i; ++j){
				SendChar('i');			
			}
			SendChar('\n');
		}*/
		osMutexRelease(uart_mutex);
	}
 }

int main (void) 
{
	osKernelInitialize (); 	// initialize CMSIS-RTOS
	
	USART1_Init ();
	uart_mutex = osMutexCreate(osMutex(uart_mutex));					//create the message queue
	T_uart1 = osThreadCreate(osThread(producer), NULL);
	T_uart2 =	osThreadCreate(osThread(consumer), NULL);
	sem1 = osSemaphoreCreate(osSemaphore(sem1), 0);	
	
	osKernelStart ();  
}
