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

#define TickScalar 100

/* taskSet 1 */
#define T1_Computation 1
#define T1_Period 3
#define T1_Deadline 3

#define T2_Computation 4
#define T2_Period 6
#define T2_Deadline 6

/* taskSet 2 */
#define T3_Computation 3
#define T3_Period 20
#define T3_Deadline 7

#define T4_Computation 2
#define T4_Period 5
#define T4_Deadline 4

#define T5_Computation 2
#define T5_Period 10
#define T5_Deadline 8

/* taskSet 3 */
#define T6_Computation 4
#define T6_Period 12
#define T6_Deadline 12

#define T7_Computation 3
#define T7_Period 9
#define T7_Deadline 9

#define T8_Computation 2
#define T8_Period 6
#define T8_Deadline 6

/* taskSet 4 */
#define T9_Computation 4
#define T9_Period 12
#define T9_Deadline 12

#define T10_Computation 3
#define T10_Period 9
#define T10_Deadline 9

#define T11_Computation 3
#define T11_Period 6
#define T11_Deadline 6


int NumberOfPeriodT1 = 1;
int NumberOfPeriodT2 = 1;
int NumberOfPeriodT3 = 1;
int NumberOfPeriodT4 = 1;
int NumberOfPeriodT5 = 1;
int NumberOfPeriodT6 = 1;
int NumberOfPeriodT7 = 1;
int NumberOfPeriodT8 = 1;
int NumberOfPeriodT9 = 1;
int NumberOfPeriodT10 = 1;
int NumberOfPeriodT11 = 1;

/* --------------------------------------------- */

void vTask1(void* /* parameter */);
void vTask2(void* /* parameter */);
void vTask3(void* /* parameter */);
void vTask4(void* /* parameter */);
void vTask5(void* /* parameter */);
void vTask6(void* /* parameter */);
void vTask7(void* /* parameter */);
void vTask8(void* /* parameter */);
void vTask9(void* /* parameter */);
void vTask10(void* /* parameter */);
void vTask11(void* /* parameter */);

void vApplicationIdleHook(void);



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
	const TickType_t xFrequency = TickScalar * T1_Period; //tsk 1 frequency
	volatile int ct = TickScalar * T1_Computation;	 //tsk 1 computation time
	while(1)
	{	
		// int tickTime_prev = xTaskGetTickCount();
		int tickTime_start = T1_Period * (NumberOfPeriodT1 - 1);
		
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
		int tickTime_deadline = T1_Period * NumberOfPeriodT1;

		printf("Task 1 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT1 += 1;
		ct = TickScalar * T1_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask2(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T2_Period; //tsk 2 frequency
	volatile int ct = TickScalar * T2_Computation;	 //tsk 2 computation time
	while(1)
	{
		// int tickTime_prev = xTaskGetTickCount();
		int tickTime_start = T2_Period * (NumberOfPeriodT2 - 1);
		
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
		int tickTime_deadline = T2_Period * NumberOfPeriodT2;

		printf("Task 2 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT2 += 1;
		ct = TickScalar * T2_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask3(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T3_Period; //tsk 3 frequency
	volatile int ct = TickScalar * T3_Computation;	 //tsk 3 computation time
	while(1)
	{
		// int tickTime_prev = xTaskGetTickCount();
		int tickTime_start = T3_Period * (NumberOfPeriodT3 - 1);
		
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
		int tickTime_deadline = T3_Period * NumberOfPeriodT3;

		printf("Task 3 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT3 += 1;
		ct = TickScalar * T3_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask4(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T4_Period;
	volatile int ct = TickScalar * T4_Computation;
	while(1)
	{
		int tickTime_start = T4_Period * (NumberOfPeriodT4 - 1);
		
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
		int tickTime_deadline = T4_Period * NumberOfPeriodT4;

		printf("Task 4 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT4 += 1;
		ct = TickScalar * T4_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask5(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T5_Period;
	volatile int ct = TickScalar * T5_Computation;
	while(1)
	{
		int tickTime_start = T5_Period * (NumberOfPeriodT5 - 1);
		
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
		int tickTime_deadline = T5_Period * NumberOfPeriodT5;

		printf("Task 5 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT5 += 1;
		ct = TickScalar * T5_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask6(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T6_Period;
	volatile int ct = TickScalar * T6_Computation;
	while(1)
	{
		int tickTime_start = T6_Period * (NumberOfPeriodT6 - 1);
		
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
		int tickTime_deadline = T6_Period * NumberOfPeriodT6;

		printf("Task 6 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT6 += 1;
		ct = TickScalar * T6_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask7(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T7_Period;
	volatile int ct = TickScalar * T7_Computation;
	while(1)
	{
		int tickTime_start = T7_Period * (NumberOfPeriodT7 - 1);
		
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
		int tickTime_deadline = T7_Period * NumberOfPeriodT7;

		printf("Task 7 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT7 += 1;
		ct = TickScalar * T7_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask8(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T8_Period;
	volatile int ct = TickScalar * T8_Computation;
	while(1)
	{
		int tickTime_start = T8_Period * (NumberOfPeriodT8 - 1);
		
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
		int tickTime_deadline = T8_Period * NumberOfPeriodT8;

		printf("Task 8 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT8 += 1;
		ct = TickScalar * T8_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask9(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T9_Period;
	volatile int ct = TickScalar * T9_Computation;
	while(1)
	{
		int tickTime_start = T9_Period * (NumberOfPeriodT9 - 1);
		
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
		int tickTime_deadline = T9_Period * NumberOfPeriodT9;

		printf("Task 9 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT9 += 1;
		ct = TickScalar * T9_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask10(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T10_Period;
	volatile int ct = TickScalar * T10_Computation;
	while(1)
	{
		int tickTime_start = T10_Period * (NumberOfPeriodT10 - 1);
		
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
		int tickTime_deadline = T10_Period * NumberOfPeriodT10;

		printf("Task 10 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT10 += 1;
		ct = TickScalar * T10_Computation;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vTask11(void* /* parameter */)
{
	TickType_t xLastWakeTime = 0;
	const TickType_t xFrequency = TickScalar * T11_Period;
	volatile int ct = TickScalar * T11_Computation;
	while(1)
	{
		int tickTime_start = T11_Period * (NumberOfPeriodT11 - 1);
		
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
		int tickTime_deadline = T11_Period * NumberOfPeriodT11;

		printf("Task 11 start %5d  |  end %5d\n", tickTime_start, tickTime_deadline);
		NumberOfPeriodT11 += 1;
		ct = TickScalar * T11_Computation;
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
	#define  taskSet_1 0
	#define  taskSet_2 0
	#define  taskSet_3 0
	#define  taskSet_4 1

	/* Creating Task with EDF Scheduling*/
	#if ( configUSE_EDF_SCHEDULER == 1 )
		#if ( taskSet_1 == 1 )
			xTaskCreate_EDF( vTask1, "vTask1", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T1_Period , T1_Deadline);
			xTaskCreate_EDF( vTask2, "vTask2", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T2_Period , T2_Deadline);
		#elif ( taskSet_2 == 1 )
			xTaskCreate_EDF( vTask3, "vTask3", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T3_Period , T3_Deadline);
			xTaskCreate_EDF( vTask4, "vTask4", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T4_Period , T4_Deadline);
			xTaskCreate_EDF( vTask5, "vTask5", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T5_Period , T5_Deadline);
		#elif ( taskSet_3 == 1 )
			xTaskCreate_EDF( vTask6, "vTask6", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T6_Period , T6_Deadline);
			xTaskCreate_EDF( vTask7, "vTask7", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T7_Period , T7_Deadline);
			xTaskCreate_EDF( vTask8, "vTask8", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T8_Period , T8_Deadline);
		#elif ( taskSet_4 == 1 )
			xTaskCreate_EDF( vTask9, "vTask9", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T9_Period , T9_Deadline);
			xTaskCreate_EDF( vTask10, "vTask10", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T10_Period , T10_Deadline);
			xTaskCreate_EDF( vTask11, "vTask11", configMINIMAL_STACK_SIZE, NULL, 1, NULL, T11_Period , T11_Deadline);
		#endif
	#else
    /* Creating Task Same Priorities and Delay*/
    xTaskCreate( vTask1, "vTask1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate( vTask2, "vTask2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate( vTask3, "vTask3", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	#endif

	vTaskStartScheduler();
	return 0;
}
