/********************************************************************
*
* NAME:
*       shmpoke.c
*
* DESCRIPTION:
*
*       This program pokes a byte into the shared memory segment
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
	struct  shmid_ds buf;
	unsigned int     crc;
	unsigned char	 *ptr;
    int     offset;
    int     value;


	if (argc != 4)  {
		printf("Usage: shmwrite <shmid> <offset> <value> \n");
		exit(-1);
	}

	id = atoi(argv[1]);                           
        offset = atoi(argv[2]);
        sscanf(argv[3],"%i", &value);

	ptr = shmat(id, NULL, 0);
	if (ptr == NULL) {
		perror("shmat error");
		exit(-1);
	}

	shmctl(id, IPC_STAT, &buf);

    if (offset >= (buf.shm_segsz - sizeof(crc))) {
        printf("error offset %d greater then shm size %d \n",
                offset, buf.shm_segsz);
        exit(-1);
    }

    /* Write to shared memory region */
    ptr[offset] = (unsigned char) (value % 0xff); 

	exit(0);
}

	
