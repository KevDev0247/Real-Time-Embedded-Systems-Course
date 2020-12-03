#include	<stdio.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/stat.h>
#include	<semaphore.h>

#define TRUE (1)
#define FALSE (0)

int main() 
{
	int i = 0;
	int childPID;
	int returnCode;
	int stat;

	char syncSemCName[] = "/processesCsync";
	char syncSemPName[] = "/processesPsync";
	
	sem_t *syncSemC, *syncSemP;
	pid_t thisChildPID;

	printf("processes\n");

	syncSemC = sem_open(syncSemCName, O_CREAT, 0700, 0);
	printf("syncSemC created\n");

	syncSemP = sem_open(syncSemPName, O_CREAT, 0700, 0);
	printf("syncSemP created\n");

	printf("Processes semaphores set up, calling fork\n");

	if ((childPID = fork()) == 0)  // Child: the following code runs on the child process
	{
		while (i < 3)
		{
			printf("Child: taking syncC semaphore\n");
			if (sem_wait(syncSemC) < 0)
			{
				perror("sem_wait syncSemC Child");
			}
			printf("Child: got syncC semaphore\n");

			printf("Child: posting syncP Parent semaphore\n");
			if (sem_post(syncSemP) < 0)
			{
				perror("sem_post syncSemP Child");
			}
			printf("Child: posted syncP Parent semaphore\n");

			i++;
		}

		printf("Child is shutting down\n");
		if (sem_close(syncSemP) < 0)
		{
			perror("sem_close syncSemP Child");
		}
		if (sem_close(syncSemC) < 0)
		{
			perror("sem_close syncSemC Child");
		}

		exit(0);
	}
	else // Parent: the following code runs on the parent process
	{
		while (i < 3) 
		{
			printf("Parent: posting syncC semaphore\n");
			if (sem_post(syncSemC) < 0)
			{
				perror("sem_post syncSemC Parent");
			}
			printf("Parent: posted syncC semaphore\n");

			printf("Parent: taking syncP semaphore\n");
			if (sem_wait(syncSemP) < 0)
			{
				perror("sem_wait syncSemP Parent");
			}
			printf("Parent: got syncP semaphore\n");

			i++;
		}

		printf("Parent is waiting for child to terminate\n");

		thisChildPID = wait(&stat);

		printf("Parent is shutting down\n");

		if (sem_close(syncSemC) < 0)
		{
			perror("sem_close syncSemC Parent");
		}
		if (sem_close(syncSemP) < 0)
		{
			perror("sem_close syncSemP Parent");
		}

		if (sem_unlink(syncSemCName) < 0)
		{
			perror("sem_unlink syncSemCName");
		}
		if (sem_unlink(syncSemPName) < 0)
		{
			perror("sem_unlink syncSemPName");
		}

		exit(0);
	}
}