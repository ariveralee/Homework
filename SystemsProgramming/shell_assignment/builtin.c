#include "myheader.h"

/* make a list of built in commands */
char *builtin_cmd[] = {
    "help",
    "cd",
    "exit",
    "clear"
};

/* make an array of function pointers what take an array of strings and return int */
int (*builtin_function[]) (char **) = {
    &myshell_help,
    &myshell_cd,
    &myshell_exit,
    &myshell_clr
};

int number_builtin() {
    return ((sizeof(builtin_cmd)) / (sizeof(char *)));
}


/* builtin functions yo. */

int myshell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "\nargument should be \"cd\" and \"..\" or path\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

int myshell_help(char **args) {
    int i;
    printf("\nAlexander Rivera's Shell Shell\n");
    printf("Type your standard arguments and then hit enter, if you try otherwise it will break... maybe ;-)\n");
    printf("\nBuilt in Functions:\n" );
    for (i = 0; i < number_builtin(); i++) {
        printf("%s\n", builtin_cmd[i]);
    }
    return 1;
}

int myshell_exit(char **args) {
    return 0;
}

int execute_cmd(char **args) {
    int i;

// if we have NULL, there's no command entered, do nothing.
    if (args[0] == NULL) {
        printf("Didn't read\n");
        return 1;
    }

// we check to see if the first argument matches our builtin commands
    for (i = 0; i < number_builtin(); i++) {
        if (strcmp(args[0], builtin_cmd[i]) == 0) {
            // if it does, we run that builtin function
            return (*builtin_function[i])(args);
        }
    }
    return launcher(args);
}



int myshell_clr(char **args) {
    if (strcmp(args[0], "clear") == 0) {
        system("clear");
    }
    return 1;
}





