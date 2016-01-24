#include <stdio.h>
#include <unilib.h>

int main() {


int i = 0;

for (; i < 5 ++i) {
    int pid = fork();

    if (!(pid != 0 && i <2 ))
        break;
        printf("Hello world\n");
}

}