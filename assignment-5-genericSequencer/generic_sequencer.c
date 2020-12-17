#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <semaphore.h>

#include <syslog.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <errno.h>

#define USEC_PER_MSEC (1000)
#define NANOSEC_PER_MSEC (1000000)
#define NANOSEC_PER_SEC (1000000000)
#define CPU_CORES_COUNT (1)
#define TRUE (1)
#define FALSE (0)

#define _GNU_SOURCE
#define THREADS_COUNT (7+1)
#define MY_CLOCK_TYPE CLOCK_MONOTONIC_RAW

int abortProgram = FALSE;
int abortS1 = FALSE, abortS2 = FALSE, abortS3 = FALSE, abortS4 = FALSE;
int semS1, semS2, semS3, semS4, semS5, semS6, semS7;
struct timespec start_time_val;

typedef struct
{
	int threadIdx;
	unsigned long long sequencePeriods;
} threadParams_t;


void *Sequencer(void* threadp);

void* Service_1(void* threadp);
void* Service_2(void* threadp);
void* Service_3(void* threadp);
void* Service_4(void* threadp);
void* Service_5(void* threadp);
void* Service_6(void* threadp);
void* Service_7(void* threadp);

double getTimeMsec(void);
double realtime(struct timespec* tsptr);
void print_scheduler(void);


void main(void)
{
	struct timespec current_time_val, current_time_res;
	double current_realtime, current_time_res;
	
	int rc, scope;
	int rt_prio_max, rt_prio_min, cpu_idx;

	cpu_set_t cpu_thread;
	cpu_set_t cpu_all;

	pthread_t threads[THREADS_COUNT];
	threadParams_t threadParams[THREADS_COUNT];
	pthread_attr_t rt_sched_attr[THREADS_COUNT];
	struct sched_param rt_param[THREADS_COUNT];

	pid_t main_pid;
	pthread_attr_t main_attr;
	struct sched_param main_param;

	clock_gettime(MY_CLOCK_TYPE, &start_time_val);
	start_realtime = realtime(&start_time_val);

	clock_gettime(MY_CLOCK_TYPE, &current_time_val);
	current_realtime = realtime(&current_time_val);

	clock_gettime(MY_CLOCK_TYPE, &current_time_res);
	current_time_res = realtime(&current_time_res);

	syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] Start High Rate Sequencer\n");
	syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] System has %d processors and %d available.\n", get_nprocs_conf(), get_nprocs());

	CPU_ZERO(&cpu_all);
	for (int i = 0; i < CPU_CORES_COUNT; i++)
	{
		CPU_SET(i, &cpu_all);
	}

	syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] Using cpu = %d from total available.\n", CPU_COUNT(&cpu_all));


	if (sem_init(&semS1, 0, 0))
	{
		printf("Failed to initialize S1 semaphore\n");
		exit(-1);
	}
	if (sem_init(&semS2, 0, 0))
	{
		printf("Failed to initialize S2 semaphore\n");
		exit(-1);
	}
	if (sem_init(&semS3, 0, 0))
	{
		printf("Failed to initialize S3 semaphore\n");
		exit(-1);
	}
	if (sem_init(&semS4, 0, 0))
	{
		printf("Failed to initialize S4 semaphore\n");
		exit(-1);
	}
	if (sem_init(&semS5, 0, 0))
	{
		printf("Failed to initialize S5 semaphore\n");
		exit(-1);
	}
	if (sem_init(&semS6, 0, 0))
	{
		printf("Failed to initialize S6 semaphore\n");
		exit(-1);
	}


	main_pid = getpid();

	rt_prio_max = sched_get_priority_max(SCHED_FIFO);
	rt_prio_min = sched_get_priority_min(SCHED_FIFO);

	rc = sched_getparam(main_pid, &main_param);
	main_param.sched_priority = rt_prio_max;
	rc = sched_setscheduler(getpid(), SCHED_FIFO, &main_param);
	if (rc < 0)
	{
		perror("main_param");
	}
	print_scheduler();

	pthread_attr_getscope(&main_attr, &scope);

	if (scope == PTHREAD_SCOPE_SYSTEM)
	{
		syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] PTHREAD SCOPE SYSTEM\n");
	}
	else if (scope == PTHREAD_SCOPE_PROCESS)
	{
		syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] PTHREAD SCOPE PROCESS\n");
	}
	else
	{
		syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] PTHREAD SCOPE UNKNOWN\n");
	}

	syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] rt_prio_max = %d\n", rt_prio_max);
	syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:5] rt_prio_min = %d\n", rt_prio_min);

	for (i = 0; i < THREADS_COUNT; i++)
	{
		if (i % 3 == 0)
		{
			CPU_ZERO(&cpu_thread);
			cpu_idx = (3);
			CPU_SET(cpu_idx, &cpu_thread);
		}
		else
		{
			CPU_ZERO(&cpu_thread);
			cpu_idx = (2);
			CPU_SET(cpu_idx, &cpu_thread);
		}

		rc = pthread_attr_init(&rt_sched_attr[i]);
		rc = pthread_attr_setinheritsched(&rt_sched_attr[i], PTHREAD_EXPLICIT_SCHED);
		rc = pthread_attr_setschedpolicy(&rt_sched_attr[i], SCHED_FIFO);
		rc = pthread_attr_setaffinity_np(&rt_sched_attr[i], sizeof(cpu_set_t), &cpu_thread);

		rc_param[i].sched_priority = rt_prio_max - 1;
		pthread_attr_setschedparam(&rt_sched_attr[i], &rt_param[i]);

		threadParams[i].threadIdx = i;
	}
}

void print_scheduler(void)
{

}
