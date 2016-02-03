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

    // we need to get the time right before we call fork.
    gettimeofday(&start, NULL);
    startTime = ((start.tv_sec + start.tv_usec) / 1000000.0);
    printf("Fork 1 Start time %d :%f\n", getpid(), startTime);

    // we get a -1 if the fork fails
    if ((pid = fork()) == -1) {
        printFailure();
    }
    // the child process
    if (pid == 0 ) {
        if (execve(APPLICATION, NULL, NULL) == -1) {
            printFailure();
        }

    } else {
        gettimeofday(&start2, NULL);
        double  startTime2 = ((start2.tv_sec + start2.tv_usec) / 1000000.0);
        printf("Fork 2 Start time %d :%f\n", getpid(), startTime2);

        // We get -1 if the fork fails.
        if ((pid2 = fork()) == -1) {
            printFailure();
        }

        // The second child will reach here, parent will skip
        if (pid2 == 0) {
            if (execve(APPLICATION, NULL, NULL) == -1) {
                printFailure();
            }
            // the parent process
        } else {
            // wait for everything to finish
            wait(&status);
        }
    }
}


void printFailure() {
    printf("Failure");
    exit(0);

}

