/********************************************************************
*
* NAME:
*       timer.c     
*
* DESCRIPTION:
*
*       This program illustrates the use of the interval timer.
*       The timer is used to generate the signal (SIGALRM) 	
*
* MODIFICATION HISTORY:
*       DATE:           AUTHOR:         PR #:/DESCRIPTION
*       12/03/04        T. Springer     Original
*
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>

#define SECS_PER_USEC    (1000000)        /* 1 second in microseconds        */

static void sig_handler(int signal);

struct itimerval startClk = {
   { 0, 0 },                               /* it_interval                    */
   { 0, 0 }                                /* it_value                       */
};

struct itimerval stopClk = {
   { 0, 0 },                               /* it_interval                    */
   { 0, 0 }                                /* it_value                       */
};

static int semid;

int main(int argc, char *argv[])
{

	struct sembuf  sops;

	double  frequency;
	int	ticks = 0;
	int	nticks;


	if (argc != 3) {
		printf("Usage: %s <ticks> <frequency> \n", argv[0]);
		exit(-1);
	}

	nticks = atoi(argv[1]);
	if ((frequency = atof(argv[2])) <= 0) {
		printf("Invalid frequency value %d \n", frequency);
		exit(-1);
	}

	/* Establish signal handler */

	signal(SIGALRM, sig_handler);

	/* Create System V semaphore */
	if ((semid = semget(ftok("timer.c", 1), 1, 0664 | IPC_CREAT)) == -1) {
                printf("Error:: semget errno #%d (%s) \n", errno,
                        strerror(errno));
                exit(-1);
        }

	sops.sem_num = 0;
        sops.sem_op = -1;
        sops.sem_flg = SEM_UNDO;

	if (frequency <= 1.0)  {	
		startClk.it_interval.tv_sec  = (unsigned long)(1.0 / frequency);
   		startClk.it_interval.tv_usec = 0;
	} else {
		startClk.it_interval.tv_sec  = 0;
                startClk.it_interval.tv_usec = (unsigned long)(SECS_PER_USEC / frequency);
	}

   	startClk.it_value.tv_sec     = 1;
   	startClk.it_value.tv_usec    = 0;

	/* Start internal timer */

	if (setitimer(ITIMER_REAL,&startClk,0) != 0) {
                printf("setitimer() -- errno #%d (%s) \n" ,errno, strerror(errno));
        	exit(-1); 
        }


	while (nticks--) {
		while (semop(semid, &sops, 1) != 0 && errno == EINTR)
			printf("Received clock tick :%d \n", ++ticks);
	}

	/* Stop the internal timer */

        if (setitimer(ITIMER_REAL,&stopClk,0) != 0) {
                printf("setitimer() -- errno #%d (%s) \n" ,errno, strerror(errno));
                exit(-1);
        }

	/* Remove the System V semaphore */

	if (semctl(semid, 0, IPC_RMID) == -1)  {
                printf("Error:: semctl() -- errno #%d (%s) \n",
                        errno, strerror(errno));
                exit(-1);
        }


}

static void sig_handler(int signo)
{

	struct sembuf  sops;

        sops.sem_num = 0;
        sops.sem_op = 1;
        sops.sem_flg = SEM_UNDO;

	if (semop(semid, &sops, 1) == -1) {
                printf("Error:: semop() -- errno #%d (%s) \n",
                        errno, strerror(errno));
                return;
        }	

	signal(SIGALRM, sig_handler); /* re-establish handler for next time */

}

