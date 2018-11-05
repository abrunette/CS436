/********************************************************************
*
* NAME:
*       shmdump.c
*
* DESCRIPTION:
*
*       This program reads a System V shared memory segment
*
* MODIFICATION HISTORY:
*       DATE:           AUTHOR:         PR #:/DESCRIPTION
*       10/21/02        T. Springer     Original
*
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>

extern void src_init(void);
extern unsigned int src_compute(char *, int);

int main(int argc, char *argv[])
{

	int	i, id;
	struct shmid_ds buf;
	unsigned char c, *ptr;
	unsigned int crc, crcbuf;

	if (argc != 2) {
		printf("Usage: shmdump <shmid> \n");
		exit(-1);
	}

	id = atoi(argv[1]);
	ptr = shmat(id, NULL, 0);
	if (ptr == NULL) {
		perror("shmat error");
		exit(-1);
	}

	if (shmctl(id, IPC_STAT, &buf) == -1) {
		perror("shmctl error");
		exit(-1);
	}

    for (i = 0; i < buf.shm_segsz; i++) {
        if (!(i % 16)) printf("\n");
           printf("%02x ", ptr[i]);
    }

    printf("\n");

	exit(0);
}

