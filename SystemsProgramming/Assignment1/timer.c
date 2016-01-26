#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#define APPLICATION "app"


int main () {
    pid_t pid = fork();
    struct timeval start;


    if (pid == 0) {
        // when you call the other program, the other program replaces you.
        
        if (execve(APPLICATION,NULL,NULL) == -1) {
            printf("Failure");
            exit(0);

        }
        
        // in the child process
        // fork returns -1  FORK FAILED if failure
        // 0 if child
        // +1 if you're the parent
    }

    else {
        //parent process
         if (fork() == 0) {
             gettimeofday(&start, NULL);
             printf("%ld\n", (start.tv_sec * 1000000 + start.tv_usec));
             execve(APPLICATION,NULL,NULL);


         }
    }
}

