#include <stdlib.h>
#include <unistd.h>

#define APPLICATION "app"
int main () {
    //char app [] = "app";
    pid_t pid = fork();

    if (pid == 0) {
        // in the child process
    }

    else {
        if (fork() == 0) {
            execve(APPLICATION,NULL,NULL);
        }
    }
}

