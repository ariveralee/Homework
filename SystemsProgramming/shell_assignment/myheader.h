#ifndef _myheader_h
#define _myheader_h

/* standard libraries for C */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* static and extern variables because it makes life easier */
static char* currentDir;
extern char** environ;

/* Function Declarations for myshell.c */
void shell_prompt();
void print_failure();
char *read_line();
char **read_args(char *);
int launcher(char **args);


/* Function Declarations for builtin.c */
int myshell_cd(char **args);
int myshell_help(char **args);
int myshell_exit(char **args);
int myshell_echo(char **args);
int number_builtin();
int execute_cmd(char **args);
int myshell_clr(char **args);

#endif