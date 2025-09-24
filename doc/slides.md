---
title: "FreeRTOS Tutorial: A Comprehensive Guide"
author: "Zhi Wang"
institute: "Iowa State University"
date: "9 / 2025"
theme: "metropolis"
colortheme: "dolphin"
fontsize: "10pt"
header-includes:
  - \usepackage{booktabs}
  - \usepackage{graphicx}
---


# Table of Contents
1. [Introduction to FreeRTOS](#introduction-to-freertos)
2. [Basic Concepts](#basic-concepts)
3. [Task Management](#task-management)
4. [Interrupt Management](#interrupt-management)
5. [Configuration](#configuration)
6. [EDF Scheduling Demo](#EDF-scheduling-demo)

# Introduction to FreeRTOS

FreeRTOS (Free Real-Time Operating System) is a popular, open-source real-time operating system kernel for embedded systems. It's designed to be small, simple, and easy to use while providing powerful features for real-time applications.

### Key Features
- **Preemptive multitasking**: Tasks can be interrupted and resumed
- **Cooperative multitasking**: Tasks voluntarily yield control
- **Multiple scheduling algorithms**: Including priority-based and EDF scheduling
- **Inter-task communication**: Queues, semaphores, mutexes
- **Memory management**: Multiple heap allocation schemes
- **Portable**: Runs on many different microcontrollers

# Basic Concepts

### Tasks
A task is a function that runs independently and has its own stack. Tasks are the fundamental building blocks of FreeRTOS applications.

### Task States
- **Running**: Currently executing on the CPU
- **Ready**: Ready to run but waiting for CPU time
- **Blocked**: Waiting for an event (delay, semaphore, etc.)
- **Suspended**: Explicitly suspended and won't run until resumed

### Scheduler
The scheduler is responsible for deciding which task runs when. FreeRTOS supports multiple scheduling policies.

# Task Management

### Creating Tasks

```c
#include "FreeRTOS.h"
#include "task.h"

// Task function prototype
void vTaskFunction(void* pvParameters);

// Task handle
TaskHandle_t xTaskHandle;

int main(void)
{
    // Create a task
    xTaskCreate(
        vTaskFunction,              // Task function
        "TaskName",                 // Task name (for debugging)
        configMINIMAL_STACK_SIZE,   // Stack size
        NULL,                       // Parameters passed to task
        1,                          // Priority (0 = lowest)
        &xTaskHandle                // Task handle
    );
    
    // Start the scheduler
    vTaskStartScheduler();
    
    return 0;
}

void vTaskFunction(void* pvParameters)
{
    while(1)
    {
        // Task code here
        printf("Task is running\n");
        
        // Delay for 1000ms
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Task Priorities
- Higher numbers = higher priority
- `configMAX_PRIORITIES` defines the maximum priority level
- Tasks with the same priority share CPU time (round-robin)

### Task Delays

```c
// Delay for a specific number of ticks
vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1000ms

// Delay until a specific time
TickType_t xLastWakeTime = xTaskGetTickCount();
vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));  // Periodic delay
```

# Scheduling Algorithms

### 1. Preemptive Priority Scheduling (Default)
- Tasks with higher priority always run first
- Lower priority tasks are preempted when higher priority tasks become ready
- Same priority tasks share CPU time

### 2. Cooperative Scheduling
- Tasks must voluntarily yield control
- No preemption occurs
- Simpler but less responsive

### 3. EDF (Earliest Deadline First) Scheduling
- Dynamic priority scheduling based on deadlines
- Task with earliest deadline gets highest priority
- More complex but optimal for real-time systems

# Configuration

### FreeRTOSConfig.h
This file contains all configuration options for FreeRTOS:

```c
// Enable preemptive scheduling
#define configUSE_PREEMPTION                    1

// Enable EDF scheduler (custom implementation)
#define configUSE_EDF_SCHEDULER                 1

// Tick rate (Hz)
#define configTICK_RATE_HZ                      (1000)

// Stack size for idle task
#define configMINIMAL_STACK_SIZE                (64)

// Total heap size
#define configTOTAL_HEAP_SIZE                   (64 * 1024)

// Maximum number of priorities
#define configMAX_PRIORITIES                    (10)

// Enable mutexes
#define configUSE_MUTEXES                       1

// Enable software timers
#define configUSE_TIMERS                        1
```

# Practical Examples

### Example 1: Basic Task Creation

Based on the project's `main.c`, here's how to create multiple tasks:

```c
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// Task functions
void vTask1(void* parameter);
void vTask2(void* parameter);

int main(void)
{
    // Create tasks with same priority
    xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    // Start scheduler
    vTaskStartScheduler();
    
    return 0;
}

void vTask1(void* parameter)
{
    while(1)
    {
        printf("Task 1 running\n");
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void vTask2(void* parameter)
{
    while(1)
    {
        printf("Task 2 running\n");
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
```

### Example 2: Periodic Tasks with vTaskDelayUntil

```c
void vPeriodicTask(void* parameter)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(300);  // 300ms period
    
    while(1)
    {
        printf("Periodic task executing\n");
        
        // Wait until next period
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
```

# EDF Scheduling

### What is EDF?
Earliest Deadline First (EDF) is a dynamic scheduling algorithm where tasks are scheduled based on their deadlines, not fixed priorities.

### EDF Implementation in This Project

The project includes a custom EDF scheduler implementation in `edf.c`. Here's how it works:

```c
// Task structure for EDF
typedef struct _task
{
    int id;     // Task ID
    int a;      // Arrival time
    int e;      // Execution time
    int p;      // Period
    int d;      // Deadline
} task;

// Example task set
task taskSet[] = {
    {1, 0, 4, 12, 12},  // Task 1: 4ms exec, 12ms period, 12ms deadline
    {2, 0, 3, 9, 9},    // Task 2: 3ms exec, 9ms period, 9ms deadline
    {3, 0, 3, 6, 6},    // Task 3: 3ms exec, 6ms period, 6ms deadline
};
```

### EDF Schedulability Test
For EDF to be schedulable, the total CPU utilization must be <= 1.0:

```c
float calculateUtilizationCPU(task *vTaskSet, int numTask) {
    float utilization = 0.0;
    for (int i = 0; i < numTask; i++) {
        utilization += (float)vTaskSet[i].e / vTaskSet[i].p;
    }
    return utilization;
}
```

### Using EDF in FreeRTOS

```c
// Enable EDF scheduler in FreeRTOSConfig.h
#define configUSE_EDF_SCHEDULER    1

// Create EDF task
xTaskCreate_EDF(
    vTaskFunction,      // Task function
    "EDFTask",          // Task name
    configMINIMAL_STACK_SIZE,
    NULL,               // Parameters
    1,                  // Priority (used for tie-breaking)
    NULL,               // Task handle
    TASK_PERIOD,        // Period
    TASK_DEADLINE       // Deadline
);
```

## Inter-Task Communication

### Queues
```c
#include "queue.h"

// Create a queue
QueueHandle_t xQueue = xQueueCreate(10, sizeof(int));

// Send data to queue
int data = 42;
xQueueSend(xQueue, &data, portMAX_DELAY);

// Receive data from queue
int receivedData;
xQueueReceive(xQueue, &receivedData, portMAX_DELAY);
```

### Semaphores
```c
#include "semphr.h"

// Create binary semaphore
SemaphoreHandle_t xSemaphore = xSemaphoreCreateBinary();

// Take semaphore
xSemaphoreTake(xSemaphore, portMAX_DELAY);

// Give semaphore
xSemaphoreGive(xSemaphore);
```

### Mutexes
```c
// Create mutex
SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();

// Take mutex (enter critical section)
xSemaphoreTake(xMutex, portMAX_DELAY);

// Critical section code here

// Give mutex (exit critical section)
xSemaphoreGive(xMutex);
```

## Best Practices

### 1. Stack Sizing
- Start with `configMINIMAL_STACK_SIZE` and increase if needed
- Use `uxTaskGetStackHighWaterMark()` to monitor stack usage
- Consider function call depth and local variables

### 2. Priority Assignment
- Use as few priority levels as possible
- Reserve highest priority for critical tasks
- Avoid priority inversion scenarios

### 3. Task Design
- Keep tasks simple and focused
- Use appropriate delays to prevent CPU hogging
- Design for preemption

### 4. Memory Management
- Choose appropriate heap allocation scheme
- Monitor heap usage
- Avoid memory leaks

### 5. Error Handling
```c
// Check return values
if (xTaskCreate(...) != pdPASS) {
    // Handle error
}

// Use configASSERT for debugging
configASSERT(xQueue != NULL);
```

# Common Pitfalls

1. **Stack Overflow**: Not allocating enough stack space
2. **Priority Inversion**: High priority task waiting for low priority task
3. **Deadlock**: Circular waiting for resources
4. **Race Conditions**: Shared data access without protection
5. **Infinite Loops**: Tasks that never yield control

# Debugging Tips

1. **Use Task Names**: Helpful for debugging and monitoring
2. **Monitor Stack Usage**: Use `uxTaskGetStackHighWaterMark()`
3. **Use Trace Facility**: Enable `configUSE_TRACE_FACILITY`
4. **Assertions**: Use `configASSERT()` for parameter validation
5. **Printf Debugging**: Use `printf()` with proper synchronization

# Conclusion

FreeRTOS provides a powerful foundation for real-time embedded applications. By understanding tasks, scheduling, and inter-task communication, you can build robust and responsive systems. The EDF scheduler extension in this project demonstrates how to implement advanced scheduling algorithms for real-time systems with strict timing requirements.

Remember to:
- Start simple and add complexity gradually
- Test thoroughly with different load conditions
- Monitor system performance and resource usage
- Follow best practices for maintainable code

For more information, refer to the official FreeRTOS documentation and the source code examples in this project.
