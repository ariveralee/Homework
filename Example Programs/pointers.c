#include <stdio.h>

/* This is a simple program show how variables are passed to functions and how to 
change the value of a variable by using its address */

void func(int);
void func1(int*);
void func2(int*);

int main(void) {
    /* instantiate a variable called x and set its value to 10. */
    int x = 10;


    /* print statement before func is called and after to demonstrate that
    passing a copy does not alter the original variable */
    printf("\n\nThe value of x before func is: %d\n", x);
    /* call the function func and pass in x */
    func(x);
    printf("The value of x after func is: %d\n\n", x);


    /* print statement before func1 is called and after to show that passing
    the address of x allows for the original value to be modified. */
    printf("The value of x before func1 is: %d\n", x);
    /* call the function func1 and pass in the address of x */
    func1(&x);
    printf("The value of x after func1 is : %d\n\n", x);
    
    
    /* second function to demonstrate further the changing of original value
    of x by using addresses and pointers */
    printf("The value of x before func2 is: %d\n", x);
    /* the &x means we want to pass the address of the variable x */
    func2(&x);
    printf("The value of x after func2 is: %d\n\n", x);


    /* return 0 if we have reached the end of the program with no problems */
    return 0;
}


/* this function is passed a copy of x, not the actual variable itself. Because of this,
once the function finishes, the value of x will still be what it was intially, 10 */
void func(int x) {
    /* multiply x by 6 and and make x that value */
    printf("The value of x in func before arthimetic is: %d\n", x);
    x *= 6;
    printf("The value of x in func after arthimetic  is: %d\n", x);
}

/* this funcction is called from main and is passed the address of variable x */
void func1(int *p) {
    /* we are multiplying the value at the address tha p is pointing to. In this
    case, p is pointing to the address of x so we are directly changing the value
    of x, not a copy. the value at the address of x (10) is being updated to the value
    of 6 * 10 which is 60. */
    
    /* we use *p to dereference the pointer meaning that we print the value that is
    located at the address that p is pointing to */
    printf("The value of x in func1 before arithmetic is: %d\n", *p);
    (*p) *= 6;
    printf("The value of x in func1 after the arithmetic is: %d\n", *p);
}

/* this function is once again passed the address of the variable x */
void func2(int *s) {
    printf("The value of x in func2 before arithmetic is: %d\n", *s);
    (*s) += 13;
    printf("The value of x in func 2 after arithmetic is: %d\n", *s);
}
