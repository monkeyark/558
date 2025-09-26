---
title: "FreeRTOS Tutorial: A Comprehensive Guide"
author: "Zhi Wang"
institute: "Iowa State University"
date: "2025 Fall"
theme: "metropolis"
colortheme: "dolphin"
fontsize: "10pt"
header-includes:
  - \usepackage{booktabs}
  - \usepackage{graphicx}
---


# Table of Contents
1. [Introduction to FreeRTOS](#introduction-to-freertos)
2. [Basic Concepts - Tasks](#basic-concepts---tasks)
3. [Basic Concepts - Configuration](#basic-concepts---configuration)
4. [EDF Scheduler Demo](#edf-scheduler-demo)
5. [Implementation Suggestions](#implementation-suggestions)
6. [Problems](#problems)
7. [Conclusion](#conclusion)

# Introduction to FreeRTOS

FreeRTOS (Free Real-Time Operating System) is a popular, open-source real-time operating system kernel for embedded systems. It's designed to be small, simple, and easy to use while providing powerful features for real-time applications.

# Introduction to FreeRTOS

## Official Documentation
https://www.freertos.org/Documentation/00-Overview

## FreeRTOS Kernel
https://github.com/FreeRTOS/FreeRTOS-Kernel


# Introduction to FreeRTOS

## Key Features
- **Preemptive multitasking**: Tasks can be interrupted and resumed
- **Cooperative multitasking**: Tasks voluntarily yield control
- **Multiple scheduling algorithms**: Including priority-based
- **Inter-task communication**: Queues, semaphores, mutexes
- **Memory management**: Multiple heap allocation schemes
- **Portable**: Runs on many different microcontrollers

# Basic Concepts - Tasks

## Tasks
A task is a function that runs independently and has its own stack. Tasks are the fundamental building blocks of FreeRTOS applications.

## Task States
- **Running**: Currently executing on the CPU
- **Ready**: Ready to run but waiting for CPU time
- **Blocked**: Waiting for an event (delay, semaphore, etc.)
- **Suspended**: Explicitly suspended and won't run until resumed

# Basic Concepts - Configuration

```c
FreeRTOSConfig.h
```
contains all configuration options for FreeRTOS

```c
// Enable preemptive scheduling
#define configUSE_PREEMPTION                    1
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
```


# Basic Concepts - Scheduling Algorithms

**Fixed Priority**: Preemptive Priority Scheduling (Default)
- Tasks with higher priority always run first
- Lower priority tasks are preempted when higher priority tasks become ready
- Same priority tasks share CPU time

**Pre-emptive**:
**Time Slicing**:



# Task Management

## Task Priorities
- Higher numbers = higher priority
- `configMAX_PRIORITIES` defines the maximum priority level
- Tasks with the same priority share CPU time (round-robin)

## Task Delays


# Task Management

## Task Functions
```c
void ATaskFunction( void *pvParameters );
```

# Task Management

## Creating a Task

```c
void vTaskFunction(void* pvParameters);

TaskHandle_t xTaskHandle;

int main(void) {
    xTaskCreate(                    // Create a task
        vTaskFunction,              // Task function
        "TaskName",                 // Task name (for debugging)
        configMINIMAL_STACK_SIZE,   // Stack size
        NULL,                       // Parameters passed to task
        1,                          // Priority (0 = lowest)
        &xTaskHandle                // Task handle
    );
    vTaskStartScheduler();          // Start the scheduler
}
```

# Task Management

## Deleting a Task

```c
void vTaskDelete( TaskHandle_t pxTaskToDelete );
```

# Task Management
## Task Priorities
- Higher numbers = higher priority
- `configMAX_PRIORITIES` defines the maximum priority level
- Tasks with the same priority share CPU time (round-robin)

## Task Delays

```c
// Delay for a specific number of ticks
vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1000ms

// Delay until a specific time
TickType_t xLastWakeTime = xTaskGetTickCount();
vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));  // Periodic delay
```


# EDF Scheduler Demo
## EDF Implementation in This Project

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

# EDF Scheduler Demo

## Using EDF in FreeRTOS

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

# Implementaion Suggestions

## 1. Stack Sizing
- Start with `configMINIMAL_STACK_SIZE` and increase if needed
- Use `uxTaskGetStackHighWaterMark()` to monitor stack usage
- Consider function call depth and local variables

## 2. Priority Assignment
- Use as few priority levels as possible
- Reserve highest priority for critical tasks
- Avoid priority inversion scenarios

# Implementaion Suggestions

## 3. Task Design
- Keep tasks simple and focused
- Use appropriate delays to prevent CPU hogging
- Design for preemption

## 4. Memory Management
- Choose appropriate heap allocation scheme
- Monitor heap usage
- Avoid memory leaks

# Implementaion Suggestions
### 5. Error Handling
```c
// Check return values
if (xTaskCreate(...) != pdPASS) {
    // Handle error
}

// Use configASSERT for debugging
configASSERT(xQueue != NULL);
```

# Problems

1. **Stack Overflow**: Not allocating enough stack space
2. **Priority Inversion**: High priority task waiting for low priority task
3. **Deadlock**: Circular waiting for resources
4. **Race Conditions**: Shared data access without protection
5. **Infinite Loops**: Tasks that never yield control

# Conclusion

FreeRTOS provides a powerful foundation for real-time embedded applications. By understanding tasks, scheduling, and inter-task communication, you can build robust and responsive systems. The EDF scheduler extension in this project demonstrates how to implement advanced scheduling algorithms for real-time systems with strict timing requirements.

For more information, refer to the official FreeRTOS documentation and the source code examples in this project.
