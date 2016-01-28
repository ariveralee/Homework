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
    struct timeval start;
    int    status;

    gettimeofday(&start, NULL);
    double startTime = ((start.tv_sec + start.tv_usec) / 1000000.0);

    // print before the first fork is called
    printf("Start time 1 is : %f\n", startTime);
    pid_t   pid = fork();

    // the child process
    if (pid  == 0 ) {
        printf("I'm the child :\n");

        if (execve(APPLICATION, NULL, NULL) == -1) {
            printFailure();
        }
    }
    // if greater than zero, we are in the parent
    else if (pid > 0) {
        gettimeofday(&start, NULL);
        double startTime2 = ((start.tv_sec + start.tv_usec) / 1000000.0);
        printf("Start time 2 is : %f\n", startTime2);
        printf("I'm the parent:\n");
        wait(&status);

        if (execve(APPLICATION, NULL, NULL) == -1) {
            printFailure();
        }
    //else, we have an error
    } else {
        printFailure();
    }

}

void printFailure() {
    printf("Failure");
    exit(0);

}

