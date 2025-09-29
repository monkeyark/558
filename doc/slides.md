---
title: "CPRE 488/588 : FreeRTOS Tutorial – A Beginner's Guide"
author: "Prof. Manimaran Govindarasu, Zhi Wang"
institute: "Iowa State University"
date: "CPRE 488/588 - 2025 Fall"
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
5. [Project Demo](#project-demo)
6. [Implementation Suggestions](#implementation-suggestions)
7. [Problems](#problems)
8. [Conclusion](#conclusion)

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

# Basic Concepts

## Tasks
A task is a function that runs independently and has its own stack. Tasks are the fundamental building blocks of FreeRTOS applications.

## Task States
- **Running**: Currently executing on the CPU
- **Ready**: Ready to run but waiting for CPU time
- **Blocked**: Waiting for an event (delay, semaphore, etc.)
- **Suspended**: Explicitly suspended and won't run until resumed

# Basic Concepts

![](fig/tskstate.png){width=70%}


# Basic Concepts

## Configuration

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

# Basic Concepts

## Scheduling Algorithms
**Fixed Priority (Default)**

- Preemptive Priority Scheduling
- Tasks with higher priority always run first
- Higher priority tasks preempt lower ones
- Same priority tasks share CPU time

**Pre-emptive Scheduling**

- Higher priority tasks can interrupt (preempt) lower priority tasks
- Critical tasks run with minimal delay

**Time Slicing (Round Robin)**

- Tasks with the same priority share CPU time equally
- Scheduler switches between these tasks at each tick interrupt

# Task Management

## Task Priorities
- Higher numbers = higher priority
- `configMAX_PRIORITIES` defines the maximum priority level
- Tasks with the same priority share CPU time (round-robin)


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


# Interrupt Management

## Interrupt Safe API

FreeRTOS provides a set of "Interrupt Safe" API functions that can be safely called from within an Interrupt Service Routine (ISR). These functions are specifically designed to handle the restrictions and requirements of running in an interrupt context.

## Key Points:
- **Do NOT call standard FreeRTOS API functions from an ISR.** Use only the versions ending with `FromISR`, such as `xQueueSendFromISR`, `xSemaphoreGiveFromISR`, or `xTaskNotifyFromISR`.
- These functions are optimized to be fast and safe for use in ISRs, and they often require an extra parameter to indicate if a context switch should be performed after the ISR.


# Project Demo - FreeRTOS EDF Scheduling

## FreeRTOS EDF Scheduling

FreeRTOS uses fixed-priority scheduling but lacks EDF support. EDF ensures better deadline adherence and resource utilization for real-time applications.

This project implements Earliest Deadline First (EDF) scheduling in FreeRTOS for IoT systems where task timing is critical and delays can cause failures or poor performance.real-time applications.

## See the project powerpoint

## https://github.com/monkeyark/558

# Project Demo

## FreeRTOS Architecture
Each real-time kernel port contains three common files and platform-specific files. The FreeRTOS_core directory contains the common kernel components, while the portable directory contains microcontroller/compiler-specific implementations.

- FreeRTOS_core/: Common kernel files (list.c, queue.c, tasks.c)
- FreeRTOS_core/portable/: Platform-specific implementations
- FreeRTOS_core/include/: Kernel header files
- FreeRTOSConfig.h: Configuration file

# Project Demo - EDF scheduler implementation edf.c

```c
// Task structure for EDF
typedef struct _task
{
    int id;     // Task ID
    int at;      // Arrival time
    int et;      // Execution time
    int pd;      // Period
    int dd;      // Deadline
} task;

// Example task set
task taskSet[] = {
    {1, 0, 4, 12, 12},  // Task 1: 4ms exec, 12ms period, 12ms deadline
    {2, 0, 3, 9, 9},    // Task 2: 3ms exec, 9ms period, 9ms deadline
    {3, 0, 3, 6, 6},    // Task 3: 3ms exec, 6ms period, 6ms deadline
};
```

# Project Demo

## EDF Scheduler Implementation

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


# Project Ideas

## Smart Home Automation
Build a system that controls lights and monitors temperature using sensors and actuators. FreeRTOS can be used to create separate tasks for sensor reading, actuator control, and communication, ensuring that each function operates independently and responds in real time to changes in the environment.

## Robot Car Controller
Develop an autonomous robot car that uses sensors for obstacle avoidance and motor control for navigation. FreeRTOS enables you to assign different tasks for sensor data acquisition, path planning, and motor actuation, allowing the robot to react quickly to obstacles and manage multiple control loops concurrently.

## IoT Data Logger
Build a data collection device using an ESP32 that reads multiple sensors, stores data locally, and transmits to cloud services via WiFi. FreeRTOS can manage tasks for periodic sensor sampling, data storage, and network communication, ensuring reliable data logging and efficient use of system resources.


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



# Reference
## Official Documentation
[1] FreeRTOS, "FreeRTOS Documentation: Overview," [Online]. Available: https://www.freertos.org/Documentation/00-Overview. [Accessed: Jun. 2024].

## FreeRTOS Kernel
[2] FreeRTOS, "FreeRTOS Kernel," GitHub repository, https://github.com/FreeRTOS/FreeRTOS-Kernel [Accessed: Jun. 2024].


# Questions

## FreeRTOS
FreeRTOS enables robust real-time embedded systems through effective task management and inter-task communication. For further details, see the official FreeRTOS documentation and project examples.