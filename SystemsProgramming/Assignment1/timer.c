#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#define APPLICATION "app"

void printFailure();

// fork returns -1  FORK FAILED if failure
// 0 if child
// > 0 if you're the parent
int main () {
    struct  timeval start;
    struct  timeval start2;
    pid_t   pid;
    pid_t   pid2;
    double  startTime;
    int     status;

    gettimeofday(&start, NULL);
    startTime = ((start.tv_sec + start.tv_usec) / 1000000.0);
    printf("Start Time 1 for PID %d :%f\n", getpid(), startTime);

    pid = fork();
    // the child process
    if (pid == 0 ) {
        printf("I'm the child: %d:\n", getpid());

        if (execve(APPLICATION, NULL, NULL) == -1) {
            printFailure();
        }

    } else {
        gettimeofday(&start2, NULL);
        double  startTime2 = ((start2.tv_sec + start2.tv_usec) / 1000000.0);
        printf("Start time 2 for PID %d :%f\n", getpid(), startTime2);

        pid2 = fork();

        if (pid2 == 0) {
            printf("I'm the child 2 %d:\n", getpid());

            if (execve(APPLICATION, NULL, NULL) == -1) {
                printFailure();
            }
            // the parent process
        } else {
            printf("I'm the parent: %d\n", getpid());
            wait(&status);
        }
    }
}


void printFailure() {
    printf("Failure");
    exit(0);

}

