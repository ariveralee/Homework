#include <stdio.h>
#include <stdlib.h>
int main() {
int i = 0;

for(i = 0; i < 2; ++i) {
    fork();
    printf(" Hello World \n");
    fflush(0);
}  
}