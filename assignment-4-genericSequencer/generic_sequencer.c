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
#define NUM_CPU_CORES (1)
#define TRUE (1)
#define FALSE (0)

#define _GNU_SOURCE
#define NUM_THREADS (7+1)
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

	pthread_t threads[NUM_THREADS];
	threadParams_t threadParams[NUM_THREADS];
	pthread_attr_t rt_sched_attr[NUM_THREADS];
	struct sched_param rt_param[NUM_THREADS];

	pid_t main_pid;
	pthread_attr_t main_attr;
	struct sched_param main_param;

	clock_gettime(MY_CLOCK_TYPE, &start_time_val);
	start_realtime = realtime(&start_time_val);

	clock_gettime(MY_CLOCK_TYPE, &current_time_val);
	current_realtime = realtime(&current_time_val);

	clock_gettime(MY_CLOCK_TYPE, &current_time_res);
	current_time_res = realtime(&current_time_res);

	syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:4] Start High Rate Sequencer\n");
	syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:4] System has %d processors and %d available.\n", get_nprocs_conf(), get_nprocs());

	CPU_ZERO(&cpu_all);
	for (int i = 0; i < NUM_CPU_CORES; i++)
	{
		CPU_SET(i, &cpu_all);
	}

	syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:4] Using cpu = %d from total available.\n", CPU_COUNT(&cpu_all));


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
		syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:4] PTHREAD SCOPE SYSTEM\n");
	}
	else if (scope == PTHREAD_SCOPE_PROCESS)
	{
		syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:4] PTHREAD SCOPE PROCESS\n");
	}
	else
	{
		syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:4] PTHREAD SCOPE UNKNOWN\n");
	}
}

void print_scheduler(void)
{

}
