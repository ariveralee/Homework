#ifndef _myheader_h
#define _myheader_h

/* standard libraries for C */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* Function Declarations for myshell.c */
void shell_loop();
void print_failure();
char *read_line();
char **read_args(char *);

/* Function Declarations for builtin.c */
int myshell_cd(char **args);
int myshell_help(char **args);
int myshell_exit(char **args);


#endif