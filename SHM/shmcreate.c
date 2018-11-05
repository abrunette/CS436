/********************************************************************
*
* NAME:
*	shmcreate.c  
*
* DESCRIPTION:
*
*	This program creates a System V shared memory segment       
*
* MODIFICATION HISTORY:
*       DATE:           AUTHOR:		PR #:/DESCRIPTION
*	10/21/02	T. Springer	Original		
*
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>

#define SVSHM_MODE	0664

int main(int argc, char *argv[])
{

	int	c, id, oflag;
	char	*ptr;
	size_t	length;

	oflag = SVSHM_MODE | IPC_CREAT;
	while ((c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
			case 'e':
				oflag |= IPC_EXCL;
				break;
		}
	}

        printf("argc %d optind %d \n", argc, optind);

	if (optind != argc - 2) {
		printf("Usage: shmget [ -e ] <key> <length> \n");
		exit(-1);
	}

	length = atoi(argv[optind + 1]);
	
	id = shmget(ftok(argv[optind], 1), length, oflag);
	if (id == -1) {
		perror("shmget error");
		exit(-1);
	}

	ptr = shmat(id, NULL, 0);
	if (ptr == NULL) {
		perror("shmat error");
		exit(-1);
	}

	exit(0);
}
	
