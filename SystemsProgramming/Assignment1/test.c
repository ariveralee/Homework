#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

int returnValue();
int i = 0;
int main () {
    int test1;
    int test2;

   if ((test1 = returnValue()) == -1) {
    printf("Failure\n");
   }

   if (test1 == 0) {
    printf("I'm the child bitch\n");
   }  else {
    if ((test2 = returnValue()) == -1) {
        printf("Failure\n");
    }
    if (test2 == 0) {
        printf("I'm the second child bitch\n");
    }
   } 







}

int returnValue() {
    return i;
}