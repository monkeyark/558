#include <stdio.h>
#include <stdlib.h>

#define STACK_PER_TASK 10
#define MIN_STACK_PER_TASK 3

typedef struct _task
{
	int id;		/* ID of the task */
	int a;		/* Arrival time of the task */
	int e;		/* Worst case execution time */
	int p;		/* Period of the task */
	int d;		/* Deadline of the task */
} task;

/* Define the task set based on the contents of file t3 */
task taskSet1[] = {
	{1, 0, 1, 3, 3},
	{2, 0, 4, 6, 6},
};

task taskSet2[] = {
	{1, 0, 3, 20, 7},
	{2, 0, 2, 5, 4},
	{3, 0, 2, 10, 8},
};

task taskSet3[] = {
	{1, 0, 4, 12, 12},
	{2, 0, 3, 9, 9},
	{3, 0, 2, 6, 6},
};

typedef struct _readyNode
{
	int taskId;
	int priority;
	int timeLeft;
	int arrival;
} readyNode;

/* Function prototypes */
int calculateHyperperiod(task *vTaskSet, int numTask);
void initializeQueue(readyNode **readyQueue, int numHyperperiod);
int updateQueue(task *vTaskSet, readyNode **readyQueue, int readyLength, int t, int numTask);
void edfSchedule(task *vTaskSet, readyNode **readyQueue, int numTask, int numHyperperiod);

/* Calculate the hyperperiod of a given task set */
int calculateHyperperiod(task *vTaskSet, int numTask)
{
	int lcm = vTaskSet[0].p;
	int i = 0;
	int a, b;

	for (i = 1; i < numTask; i++) {
		a = lcm;
		b = vTaskSet[i].p;
		while (a != b) {
			if (a > b)
				a = a - b;
			else
				b = b - a;
		}
		lcm = (lcm * vTaskSet[i].p) / a;
	}
	return lcm;
}

/* Initialize the ready queue */
void initializeQueue(readyNode **readyQueue, int numHyperperiod)
{
	*readyQueue = (readyNode *)malloc(sizeof(readyNode) * numHyperperiod);
	int i = 0;

	for (i = 0; i < numHyperperiod; i++)
		(*readyQueue)[i].taskId = -1;
}

int updateQueue(task *vTaskSet, readyNode **readyQueue, int readyLength, int t, int numTask)
{
	for (int i = 0; i < numTask; i++) {
		/* Check if a process has to enter the ready queue */
		if (((t - vTaskSet[i].a) % vTaskSet[i].p == 0)) {
			/* Add the task to the ready queue */
			readyLength++;
			(*readyQueue)[readyLength].taskId = vTaskSet[i].id;
				(*readyQueue)[readyLength].priority = -1 * (t + vTaskSet[i].d);
				(*readyQueue)[readyLength].timeLeft = vTaskSet[i].e;
				(*readyQueue)[readyLength].arrival = t;
		}
	}
	return readyLength;
}

void edfSchedule(task *vTaskSet, readyNode **readyQueue, int numTask, int numHyperperiod)
{
	int worstStack = 0;
	int readyLength = -1;
	int t = 0;
	int i = 0;

	for (t = 0; t < numHyperperiod; t++) {
		/* Update the ready queue */
		readyLength = updateQueue(vTaskSet, readyQueue, readyLength,
					t, numTask);

		int index = -1;
		int taskId = -1;
		int maxPriority = -100;
		int currentStack = 0;

		/* Select a suitable process from the ready queue */
		for (i = 0; i <= readyLength; i++) {
			if ((*readyQueue)[i].taskId != -1) {
				if ((*readyQueue)[i].priority > maxPriority) {
					index = i;
					taskId = (*readyQueue)[i].taskId;
					maxPriority = (*readyQueue)[i].priority;
				}
			}
		}

		/* Decrease the time left for the selected task */
		(*readyQueue)[index].timeLeft--;

		/* Calculating the current stack space required */
		for (i = 0; i <= readyLength; i++) {
			if ((*readyQueue)[i].taskId != -1) {
				if ((*readyQueue)[i].timeLeft <
				    vTaskSet[(*readyQueue)[i].taskId - 1].e)
					currentStack += STACK_PER_TASK;
				else
					currentStack += MIN_STACK_PER_TASK;
			}
		}

		/* Update the task in the ready queue */
		if (currentStack > worstStack)
			worstStack = currentStack;

		/* Remove the task from the ready queue if it is completed */
		if ((*readyQueue)[index].timeLeft == 0)
			(*readyQueue)[index].taskId = -1;

		/* Print the schedule */
		if (taskId == -1)
			// printf("Time: %d ----> IDLE | Stack Space: %d\n", t, current_stack); //DEBUG
			printf("Time: %3d ----> Task: IDLE\n", t);
		else
			// printf("Time: %3d ----> Task: %d | Stack Space: %d\n", t, task_id, current_stack); //DEBUG
			printf("Time: %3d ----> Task: %d\n", t, taskId);
	}
	// printf("Worst Case Stack Space needed: %d frames\n", worstStack); //DEBUG
}

int main()
{
	/* Choose the task set */
	task *currentTaskSet = taskSet3;
	int numTask = sizeof(taskSet3) / sizeof(task);

	// printf("Total tasks in the system: %d\n", numTask); //DEBUG

	/* Calculate the hyperperiod of the task set */
	int numHyperperiod = calculateHyperperiod(currentTaskSet, numTask);

	printf("Hyperperiod: %d\n", numHyperperiod);

	/* Initialize the ready queue */
	readyNode *readyQueue;

	initializeQueue(&readyQueue, numHyperperiod);

	/* Schedule the tasks */
	edfSchedule(currentTaskSet, &readyQueue, numTask, numHyperperiod);

	free(readyQueue);
	return 0;
}
