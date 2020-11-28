#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>

typedef struct
{
   int threadIdx;
} threadParams_t;

pthread_t threads[1];
threadParams_t threadParams[1];

void *pthread(void *threadp) 
{
   syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Thread!\n");
}

int main (int argc, char *argv[])
{
   int rc;
   int i = 0;
   
   syslog(LOG_INFO, system("uname -a"));
   syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Main!\n");

   threadParams[i].threadIdx = i;
   pthread_create(&threads[i],
		  (void *)0, 
		  pthread, 
		  (void *)&(threadParams[i])
		 );

   pthread_join(threads[i], NULL);
   
   closelog();
}
