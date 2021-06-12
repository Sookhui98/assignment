#include "STM32F10x.h"
#include "cmsis_os.h"

#include "Board_LED.h"

osMessageQId Q_LED;																		//define the message queue
osMessageQDef (Q_LED,0x16,unsigned char);
osEvent  result;

void producer (void const *argument);																
void consumer (void const *argument);
int BUFFER_SIZE [4];
int bufferlength = 0;
int readpointer = 0;
int writepointer = 0;

osThreadDef(producer, osPriorityNormal, 1, 0);
osThreadDef(consumer, osPriorityAboveNormal, 1, 0);

osThreadId T_producer;
osThreadId T_consumer;
osThreadId	T_main; 


void consumer (void const *argument) 
{
	for (;;) 
	{
		result = 	osMessageGet(Q_LED,osWaitForever);				//wait for a message to arrive
		LED_On(0x01);		// write the data to the LED's
    osDelay(10);
		result = 	osMessageGet(Q_LED,osWaitForever);	
		LED_On(0x02);
		osDelay(10);
		result = 	osMessageGet(Q_LED,osWaitForever);	
		LED_On(0x03);
		osDelay(10);
		result = 	osMessageGet(Q_LED,osWaitForever);	
		LED_On(0x04);
		osDelay(10);
	}
}

void producer (void const *argument) 
{
	for (;;) 
	{
		osMessagePut(Q_LED,0x01, osWaitForever);			//send message
		LED_Off(0x01);		// off the LED
    osDelay(10);
		osMessagePut(Q_LED,0x02, osWaitForever);	
		LED_Off(0x02);
		osDelay(10);
		osMessagePut(Q_LED,0x03, osWaitForever);	
		LED_Off(0x03);
		osDelay(10);
		osMessagePut(Q_LED,0x04, osWaitForever);	
		LED_Off(0x04);
		osDelay(10);
	}
}

int main (void) 
{
	osKernelInitialize ();                    						// initialize CMSIS-RTOS
	
	LED_Initialize ();
	Q_LED = osMessageCreate(osMessageQ(Q_LED),NULL);					//create the message queue
	T_producer = osThreadCreate(osThread(producer), NULL);
	T_consumer =	osThreadCreate(osThread(consumer), NULL);
	
	osKernelStart ();  
}