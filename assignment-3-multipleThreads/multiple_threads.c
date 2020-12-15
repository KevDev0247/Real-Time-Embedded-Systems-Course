#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>

#define THREAD_COUNT 128

typedef struct
{
	int threadIdx;
} threadParams_t;

pthread_t threads[THREAD_COUNT];
threadParams_t threadParams[THREAD_COUNT];

void* counterThread(void* threadp)
{
	int i;
    int gsum = 0;
	threadParams_t* threadParams = (threadParams_t*)threadp;

	for (i = 0; i <= threadParams->threadIdx; i++)
	{
		gsum += i;
	}
 
    syslog(LOG_INFO, "[COURSE:5623][ASSIGNMENT:1]: Thread idx=%d, sum[1...%d]=%d\n", threadParams->threadIdx, threadParams->threadIdx, gsum);
}

int main(int argc, char* argv[])
{
	int rc;
	int i;
 
    for (i = 0; i < THREAD_COUNT; i++)
    {
      threadParams[i].threadIdx = i;
  	  pthread_create(&threads[i],
  	    	(void*)0,
  		    counterThread,
  		    (void*)&(threadParams[i]));
    }

	for (i = 0; i < THREAD_COUNT; i++)
	{
		pthread_join(threads[i], NULL);
	}
}