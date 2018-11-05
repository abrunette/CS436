/*****************filename**************/
//Author: Aaron Brunette
//Github: abrunette
//Last updated: 2018/05/09
//Compiled with gcc
//Written on Vim
//Purpose: To send a requested file from a server to a client.
/***************************************/

//This is not complete or functional, but this is as far as I got.

//General
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <pthread.h>

//Networking
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

//Macros
#define MAXLINE	256
#define BUFFER 1024

//Globals
const int backlog = 4;
char file[] = "";
struct sockaddr_in cliaddr;

//Prototype
void *client_request(void *arg);

int main(int argc, char *argv[])
{

    int	    listenfd, connfd;
    pid_t   childpid;
    int     clilen;
    struct  sockaddr_in cliaddr, servaddr;
	pthread_t tid;

    if (argc != 4) {
	printf("Usage: tcpserver <address> <port> <file.type>\n");
	return EXIT_FAILURE;
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
	perror("Error initializing socket. Exiting...\n");
	return EXIT_FAILURE;
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family        = AF_INET;
    servaddr.sin_addr.s_addr   = inet_addr(argv[1]);
    servaddr.sin_port          = htons(atoi(argv[2]));
    file[256] = argv[3];

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
	perror("Error binding socket. Exiting...\n");
        return EXIT_FAILURE;

    }
	
    if (listen(listenfd, backlog) == -1) {
	perror("Error listening for request. Exiting...\n");
	return EXIT_FAILURE;
    }

    while (1) {
	clilen = sizeof(cliaddr);
	if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0 ) {
		if (errno == EINTR)
			continue;
		else {
			perror("Error accepting request. Exiting...\n");
			return EXIT_FAILURE;
		}
	}
	pthread_create(&tid, NULL, &client_request, (void *)&connfd);
    }
	close(connfd);
	return 0;
}

void *client_request(void *arg)
{
	int connfd = *(int *)arg;
	int fileSize;

	write(connfd, file, MAXLINE);
	
	printf("the file name: %s", file);

	FILE *file = fopen(file, "r");
	if(file == NULL)
	{
		perror("Could not open file. Exiting...\n");
		return EXIT_FAILURE;
	}

	while (1) {
		char fileBuffer[BUFFER] = {0};		
		fileSize = fread(fileBuffer, 1, BUFFER, file);
		if(fileSize > 0)
	    		write(connfd, BUFFER, strlen(BUFFER));
	}
	close(connfd);
}
