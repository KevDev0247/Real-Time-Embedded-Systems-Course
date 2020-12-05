#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>


#define NUM_THREADS 64
#define NUM_CPUS 4

typedef struct
{
	int threadIdx;
} threadParams_t;

pthread_t threads[NUM_THREADS];
pthread_t mainThread;
pthread_t startThread;
threadParams_t threadParams[NUM_THREADS];

pthread_attr_t fifo_sched_attr;
pthread_attr_t orig_sched_attr;
struct sched_param fifo_param;

#define SCHED_POLICY SCHED_FIFO
#define MAX_ITERATIONS (1000000)



void print_scheduler(void)
{
	int schedType = sched_getscheduler(getpid());

	switch (schedType)
	{
		case SCHED_FIFO:
			printf("Pthread policy is SCHED_FIFO\n");
			break;
		case SCHED_OTHER:
			printf("Pthread policy is SCHED_OTHER\n");
			break;
		case SCHED_RR:
			printf("Pthread policy is UNKNOWN\n");
			break;
		default:
			printf("Pthread policy is UNKNOWN\n");
	}
}

void set_scheduler(void)
{
	int max_prio, scope, rc, cpuidx;
	cpu_set_t cpuset;

	printf("INITIAL ");
	print_scheduler();

	pthread_attr_init(&fifo_sched_attr);
	pthread_attr_setinheritsched(&fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&fifo_sched_attr, SCHED_POLICY);

	CPU_ZERO(&cpuset);
	cpuidx = (3);
	CPU_SET(cpuidx, &cpuset);
	pthread_attr_setaffinity_np(&fifo_sched_attr, sizeof(cpu_set_t), &cpuset);

	max_prio = sched_get_priority_max(SCHED_POLICY);
	fifo_param.sched_priority = max_prio;

	if ((rc = sched_setscheduler(getpid(), SCHED_POLICY, &fifo_param)) < 0) 
	{
		perror("sched_setscheduler");
	}

	pthread_attr_setschedparam(&fifo_sched_attr, &fifo_param);

	printf("ADJUSTED ");
	print_scheduler();
}

void* counterThread(void *threadp)
{
	int sum = 0, i, j, rc;

	threadParams_t* threadParams = (threadParams_t *)threadp;
	pthread_t mythread;

	double start = 0.0, stop = 0.0;
	struct timeval startTime, stopTime;
	
	gettimeofday(&startTime, 0);
	start = ((startTime.tv_sec * 1000000.0) + startTime.tv_usec) / 1000000.0;

	for (i = 0; i < MAX_ITERATIONS; i++) 
	{
		sum = 0;
		for (j = 0; j < (threadParams->threadIdx) + 1; j++)
		{
			sum = sum + 1;
		}
	}

	gettimeofday(&stopTime, 0);
	stop = ((stopTime.tv_sec * 1000000.0) + stopTime.tv_usec) / 1000000.0;

	printf("\nThread idx = %d, sum[0...%d] = %d, running on CPU = %d, start = %lf, stop = %lf",
		   threadParams->threadIdx, 
		   threadParams->threadIdx, 
		   sum, sched_getcpu(), 
		   start, stop);
}

void *starterThread(void *threadp)
{
	int i, rc;

	printf("starter thread running on CPU = %d\n", sched_getcpu());

	for (i = 0; i < NUM_THREADS; i++)
	{
		threadParams[i].threadIdx = i;

		pthread_create(&threads[i],
				       &fifo_sched_attr,
				       counterThread, 
				       (void*)&(threadParams[i]));
	} 

	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
}
 
int main(int argc, char *argv[]) 
{
	int rc;
	int i, j;
	cpu_set_t cpuset;

	set_scheduler();
	
	CPU_ZERO(&cpuset);

	mainThread = pthread_self();
	rc = pthread_getaffinity_np(mainThread, sizeof(cpu_set_t), &cpuset);

	if (rc != 0)
	{
		perror("pthread_getaffinity_np");
	}
	else
	{
		printf("main thread running on CPU=%d, CPUs =", sched_getcpu());

		for (j = 0; j < CPU_SETSIZE; j++)
		{
			if (CPU_ISSET(j, &cpuset))
			{
				printf(" %d", j);
			}
		}
		printf("\n");
	}

	pthread_create(&startThread,
		           &fifo_sched_attr,
		           starterThread, 
		           (void*)0);

	pthread_join(startThread, NULL);

	printf("\nTEST COMPLETE\n");
}