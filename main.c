/*
 * main() creates all the demo application tasks, then starts the scheduler.
 * The web documentation provides more details of the standard demo application
 * tasks, which provide no particular functionality but do provide a good
 * example of how to use the FreeRTOS API.
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Check" task - This only executes every five seconds but has a high priority
 * to ensure it gets processor time.  Its main function is to check that all the
 * standard demo tasks are still operational.  While no errors have been
 * discovered the check task will print out "OK" and the current simulated tick
 * time.  If an error is discovered in the execution of a task then the check
 * task will print out an appropriate error message.
 *
 */


/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"


/* Examples */
// T1 = (4, 12); T2 = (3, 9); T3 = (2, 6)
// T4 = (3, 8); T5 = (1, 4); T6 = (1, 4); T7 = (2, 6)

#define TickScalar 100

#define ComputT1 4
#define ComputT2 3
#define ComputT3 2

#define ComputT4 3
#define ComputT5 1
#define ComputT6 1
#define ComputT7 2

#define FreqT1 12
#define FreqT2 9
#define FreqT3 6

#define FreqT4 8
#define FreqT5 4
#define FreqT6 4
#define FreqT7 6


/* --------------------------------------------- */

void vTask1(void* /* parameter */);
void vTask2(void* /* parameter */);
void vTask3(void* /* parameter */);
void vTask4(void* /* parameter */);
void vTask5(void* /* parameter */);
void vTask6(void* /* parameter */);
void vTask7(void* /* parameter */);


void vApplicationIdleHook(void);

int NumberOfPeriodT1 = 1;
int NumberOfPeriodT2 = 1;
int NumberOfPeriodT3 = 1;
int NumberOfPeriodT4 = 1;
int NumberOfPeriodT5 = 1;
int NumberOfPeriodT6 = 1;
int NumberOfPeriodT7 = 1;


void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
	taskENTER_CRITICAL();
	{
		printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
		fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}


#if ( configUSE_EDF_SCHEDULER == 0 )
void vTask1(void* parameter)
{
	while(1){
		printf("Task 1\n");
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}
void vTask2(void* parameter)
{
	while(1){
		printf("Task 2\n");
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}
void vTask3(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
	while(1){
		printf("Task 3 with 250ms\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(250));
	}
}
void vTask4(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
	while(1){
		printf("Task 4 with 300ms\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(300));
	}
}
#else
void vTask1(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * FreqT1; //tsk 1 frequency
	volatile int ct = TickScalar * ComputT1;	 //tsk 1 computation time
	while(1)
	{	
		// int tickTime_prev = xTaskGetTickCount();
		int tickTime_start = FreqT1 * (NumberOfPeriodT1 - 1);
		
		TickType_t xTime = xTaskGetTickCount();
		TickType_t x;
		while(ct != 0)
		{
			if((x = xTaskGetTickCount()) > xTime)
			{
				xTime = x;
				ct--;
			}
		}
		// int tickTime_current = xTaskGetTickCount();
		int tickTime_deadline = FreqT1 * NumberOfPeriodT1;

		printf("Task 1 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT1 += 1;
		ct = TickScalar * ComputT1;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask2(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * FreqT2; //tsk 2 frequency
	volatile int ct = TickScalar * ComputT2;	 //tsk 2 computation time
	while(1)
	{
		// int tickTime_prev = xTaskGetTickCount();
		int tickTime_start = FreqT2 * (NumberOfPeriodT2 - 1);
		
		TickType_t xTime = xTaskGetTickCount();
		TickType_t x;
		while(ct != 0)
		{
			if((x = xTaskGetTickCount()) > xTime)
			{
				xTime = x;
				ct--;
			}
		}
		// int tickTime_current = xTaskGetTickCount();
		int tickTime_deadline = FreqT2 * NumberOfPeriodT2;

		printf("Task 2 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT2 += 1;
		ct = TickScalar * ComputT2;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask3(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * FreqT3; //tsk 3 frequency
	volatile int ct = TickScalar * ComputT3;	 //tsk 3 computation time
	while(1)
	{
		// int tickTime_prev = xTaskGetTickCount();
		int tickTime_start = FreqT3 * (NumberOfPeriodT3 - 1);
		
		TickType_t xTime = xTaskGetTickCount();
		TickType_t x;
		while(ct != 0)
		{
			if((x = xTaskGetTickCount()) > xTime)
			{
				xTime = x;
				ct--;
			}
		}
		// int tickTime_current = xTaskGetTickCount();
		int tickTime_deadline = FreqT3 * NumberOfPeriodT3;

		printf("Task 3 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT3 += 1;
		ct = TickScalar * ComputT3;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

#endif



void vApplicationIdleHook(void)
{
//	printf("Idle\r\n");
}
/*-----------------------------------------------------------*/

int main ( void )
{
	// T1 = (4, 12); T2 = (3, 9); T3 = (2, 6) 
	/* Creating Task with EDF Scheduling*/
	#if ( configUSE_EDF_SCHEDULER == 1 )
	xTaskPeriodicCreate( vTask1, "vTask1", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 12 );
	xTaskPeriodicCreate( vTask2, "vTask2", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 9 );
	xTaskPeriodicCreate( vTask3, "vTask3", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 6 );
	#else
    /* Creating Task Same Priorities and Delay*/
    xTaskCreate( vTask1, "vTask1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate( vTask2, "vTask2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate( vTask3, "vTask3", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	#endif

	vTaskStartScheduler();
	return 0;
}
