#include <stdio.h>
#include <stdlib.h>

#define STACK_PER_TASK 10
#define MIN_STACK_PER_TASK 3


/*
taskset1
3
0 4 1 -1
0 5 2 -1
0 20 7 -1
*/


typedef struct _task
{
	int id; //ID of the task
	int a; //Arrival time of the task
	int P; //Period of the task
	int e; //Worst case execution time
	int d; //Relative deadline
} task;

typedef struct _ready_node
{ 
	int task_id; // Task/job number
	int priority; // Priority of the task
	int time_left; // Execution time left
	int arrival; // Arrival time (in the Queue)
} ready_node;

//Read the task set from STDIN
int read_taskset(task **taskset)
{
	int N; //Total number of tasks in the system
	//Read the total number of tasks
	scanf("%d",&N);
	// allocate memory
	*taskset = (task*)malloc(sizeof(task) * N);
	//read the individual task parameters
	int i = 0;
	int value; // Variable used to read parameter values
	for(i = 0 ; i < N ; i++)
	{

		//ID
		(*taskset)[i].id = i + 1;
		
		//Arrival time
		scanf("%d", &value);
		(*taskset)[i].a = value == -1 ? 0 : value;

		//Period of the task
		scanf("%d", &value);
		(*taskset)[i].P = value;

		//Worst case execution time
		scanf("%d", &value);
		(*taskset)[i].e = value;

		//Relative deadline
		scanf("%d", &value);
		(*taskset)[i].d = value == -1 ? (*taskset)[i].P : value;		

	}

	return N;
}

//Calculate the hyperperiod of a given task set
int hyperperiod(task *taskset, int N)
{
	int lcm = taskset[0].P;
	int i = 0;
	int a, b;
	for(i = 1 ; i < N ; i++)
	{
		a = lcm;
		b = taskset[i].P;
		while(a != b)
		{
			if (a > b)
				a = a - b;
			else
				b = b - a;
		}
		
		lcm = (lcm * taskset[i].P) / a;
	}
	return lcm;
} 


//Initialize the ready queue.
void initialize_queue(ready_node **ready_queue, int H)
{
	*ready_queue = (ready_node*)malloc(sizeof(ready_node) * H);
	int i = 0;
	for( i = 0 ; i < H; i++)
	{
		(*ready_queue)[i].task_id = -1; //Indicating there is no job arriving at this time
	}
}

//update the ready queue at after every unit of time
int update_queue(task **taskset, ready_node **ready_queue, int ready_length, int t, int N)
{
	int i = 0;
	for( i = 0 ; i < N ; i++)
	{
		//Check if a process has to enter the ready queue
		if ( ( (t - (*taskset)[i].a) % (*taskset)[i].P == 0 ))
		{
			//Add the task to the ready queue
			ready_length++;
			(*ready_queue)[ready_length].task_id = (*taskset)[i].id;
			(*ready_queue)[ready_length].priority = -1 * (t + (*taskset)[i].d);
			(*ready_queue)[ready_length].time_left = (*taskset)[i].e;
			(*ready_queue)[ready_length].arrival = t;
		}
	}

	return ready_length;
}

void print_ready_queue(ready_node *queue, int ready_length)
{
	int i = 0;
	for (i = 0 ; i <= ready_length ; i++)
	{
		printf("ID: %d | Priority: %d | Time Left: %d | Arrival: %d\n", queue[i].task_id, queue[i].priority, queue[i].time_left, queue[i].arrival);
	}
}

void edf_sched(task **taskset, ready_node **ready_queue, int N, int H)
{
	int worst_stack = 0;
	int ready_length = -1;
	int t = 0;
	int i = 0;
	printf("Debug: Starting EDF scheduling\n");
	for( t = 0 ; t < H; t++)
	{
		printf("Debug: Time %d\n", t);
		//Update the ready queue
		
		ready_length = update_queue(taskset, ready_queue, ready_length, t, N);

		printf("Debug: Ready queue updated, length: %d\n", ready_length);

		//printf("---------------------READY QUEUE------------------------\n");
		//print_ready_queue(*ready_queue, ready_length);

		int index = -1; //Index of the task that has to run
		int task_id = -1; //ID of the task that has to run
		int max_priority = -100; //Store the priority of the selected task
		int current_stack = 0;

		//Select a suitable process from the ready queue
		for( i = 0 ; i <= ready_length ; i++)
		{
			if ( (*ready_queue)[i].task_id != -1 )
			{
				if ((*ready_queue)[i].priority > max_priority)
				{	
					index = i;
					task_id = (*ready_queue)[i].task_id;
					max_priority = (*ready_queue)[i].priority;
				}
			}
		}

		printf("Debug: Selected task ID: %d\n", task_id);

		//Decrease the time left for the selected task
	    (*ready_queue)[index].time_left--;

		//Calculating the current stack space required
		for( i = 0 ; i <= ready_length ; i++)
		{
			if ( (*ready_queue)[i].task_id != -1 )
			{
				if ((*ready_queue)[i].time_left < (*taskset)[(*ready_queue)[i].task_id - 1].e)
					current_stack += STACK_PER_TASK;
				else
					current_stack += MIN_STACK_PER_TASK;
			}
		}			

		printf("Debug: Current stack space: %d\n", current_stack);

		//Update the task in the ready queue (i.e update the time left)
		if (current_stack > worst_stack)
				worst_stack = current_stack;
		
		//Remove the task from the ready queue if it is completed
		if ( (*ready_queue)[index].time_left == 0 )
			(*ready_queue)[index].task_id = -1;

		//Print the schedule
		if (task_id == -1)
			printf("Time: %d ----> IDLE (Task ID: %d) | Stack Space: %d\n", t, task_id, current_stack);
		else
			printf("Time: %d ----> Task: %d | Stack Space: %d\n", t, task_id, current_stack);
	}
	printf("Debug: EDF scheduling completed\n");
	printf("Worst Case Stack Space needed: %d frames\n", worst_stack);
}

int main()
{
	//Read the edf task set
	task *taskset; 
	int N = read_taskset(&taskset);
	printf("Total tasks in the system: %d\n", N);

	//Calculate the hyperperiod of the task set
	int H = hyperperiod(taskset, N);
	printf("Hyperperiod: %d\n", H);
	
	//Initialize the ready queue
	ready_node *ready_queue;
	initialize_queue(&ready_queue, H);

	//Schedule the tasks
	edf_sched(&taskset, &ready_queue, N, H);

	free(taskset);
	free(ready_queue);
	return 0;
}
