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
// T1 = (3, 8); T2 = (1, 4); T3 = (1, 4); T4 = (2, 6)
// T1 = (4, 12); T2 = (3, 9); T3 = (2, 6)

#define Count 20
#define AFreq 50
#define Bfreq 80

/* --------------------------------------------- */
#if ( configUSE_EDF_SCHEDULER == 0 )
void vTask1(void*);
void vTask2(void*);
void vTask3(void*);
void vTask4(void*);
#else
void vTask1(void* /* parameter */);
void vTask2(void* /* parameter */);
#endif


void vApplicationIdleHook(void);

int ANumberOfPeriod = 1;
int BNumberOfPeriod = 1;


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
    TickType_t xLastWakeTimeA;
    const TickType_t xFrequency = AFreq; //tsk A frequency
    volatile int count = Count;     //tsk A capacity
    xLastWakeTimeA = 0;
    while(1)
    {	
        printf("Tick %3d: Task 1 In with start  %3d\n", xTaskGetTickCount (), AFreq * (ANumberOfPeriod - 1));
        TickType_t xTime = xTaskGetTickCount ();
        TickType_t x;
        while(count != 0)
        {
            if(( x = xTaskGetTickCount () ) > xTime)
            {
                xTime = x;
                count--;
            }
        }
        printf("Tick %3d: Task 1 Out with deadLine  %3d\n", xTaskGetTickCount (), AFreq* ANumberOfPeriod);
        ANumberOfPeriod += 1;
        count = Count;
        vTaskDelayUntil( &xLastWakeTimeA, xFrequency );
    }
}
void vTask2(void* /* parameter */)
{
    TickType_t xLastWakeTimeB;
    const TickType_t xFrequency = Bfreq; //tsk B frequency
    volatile int count = 2*Count;     //tsk B capacity
    xLastWakeTimeB = 0;
    while(1)
    {
        printf("Tick %3d: Task 2 In with start  %3d\n", xTaskGetTickCount (), Bfreq*(BNumberOfPeriod - 1));
        TickType_t xTime = xTaskGetTickCount ();
        TickType_t x;
        while(count != 0)
        {
            if(( x = xTaskGetTickCount () ) > xTime)
            {
                xTime = x;
                count--;
            }
        }
        printf("Tick %3d: Task 2 Out with deadLine  %3d\n", xTaskGetTickCount (), Bfreq*BNumberOfPeriod);
        BNumberOfPeriod += 1;
        count = 2*Count;
        vTaskDelayUntil( &xLastWakeTimeB, xFrequency );
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
	/* Creating Two Task Same Priorities and Delay*/
//	xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//	xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

	/* Creating Two Task Same Priorities and DelayUntil*/
	#if ( configUSE_EDF_SCHEDULER == 1 )
	xTaskPeriodicCreate( vTask1, "vTask1", 1000, NULL, 1, NULL, 5 );
	xTaskPeriodicCreate( vTask2, "vTask2", 1000, NULL, 1, NULL, 8 );
	#else
	xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL );
	xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
	#endif

	vTaskStartScheduler();
	return 0;
}
