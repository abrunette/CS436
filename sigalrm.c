#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


static int alarm_fired = 0;


void sig_handler(int sig)
{

    if (sig == SIGALRM)  {
        printf("Got the signal !! \n");
        alarm_fired = 1;
    }

}

int main(int argc, char *argv[])
{


    struct timeval tp;
   
    pid_t    pid;
    double   t1, t2;

    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return(-1);
    }

    if (pid == 0) {
        signal(SIGALRM, sig_handler);
        gettimeofday(&tp, NULL);
        t1 = (double)(tp.tv_sec + 
                      (double)(tp.tv_usec * 1e-06));

        pause();
        if (alarm_fired) {
            gettimeofday(&tp, NULL);
            t2 = (double)(tp.tv_sec + 
                         (double)(tp.tv_usec * 1e-06));
        
            printf("elapsed msecs: %d \n", (int)((t2 - t1) * 1e+06));
        }

        return(0);

    }

    gettimeofday(&tp, NULL);
    printf("Time %d.%d \n", tp.tv_sec, tp.tv_usec);
    kill(pid, SIGALRM);

    wait();

    return(0);

}
    
