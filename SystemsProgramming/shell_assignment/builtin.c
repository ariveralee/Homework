#include "myheader.h"

/* make a list of built in commands */
char *builtin_cmd[] = {
    "help",
    "cd",
    "exit",
    "clear",
    "echo"
};

/* make an array of function pointers what take an array of strings and return int */
int (*builtin_function[]) (char **) = {
    &myshell_help,
    &myshell_cd,
    &myshell_exit,
    &myshell_clr,
    &myshell_echo
};

int number_builtin() {
    return ((sizeof(builtin_cmd)) / (sizeof(char *)));
}


/* builtin functions yo. */

int myshell_cd(char **args) {
    // if there's no second argument, just return home
    if (args[1] == NULL) {
        chdir(getenv("HOME"));
        return 1;
        // else there is a second argument, try to find the path print error message if there is none
    } else {
        if (chdir(args[1]) != 0) {
            printf("%s: no such directory exists\n", args[1]);
            return 1;
        }
    }
    return 0;
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

/* this function is for the executing of the builtin commands.
 * if there's nothing enter, there's no commands so we do nothing
 * else, there is a command so we compare it to the list of builtin
 * commands that we have. If there's a match then we call that builtin
 * command. However, if there is not a match, we assume we need to launch
 * something else so we pass these arguments to the launcher.
 */
int execute_cmd(char **args) {
    int i;
// if we have NULL, there's no command entered, do nothing.
    if (args[0] == NULL) {
        printf("\n");
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

/* This function is for when the user has entered clear, to cheat we 
 * use the system call "clear" 
 */
int myshell_clr(char **args) {
    if (strcmp(args[0], "clear") == 0) {
        system("clear");
    }
    return 1;
}

/* This function is my implementation of echo. If the first argument
 * passed is "echo" then we know to mimic the behavior of echo. First
 * ,we check to see if there's an arg[1], if not a newline is printed
 * just as the system shell does. Else, we know there's something to 
 * print, but where do we stop? We stop once we have no more arugments
 * to print.
 */
int myshell_echo(char **args) {
    // if the first arg matches echo, then it's the echo command.
    if (strcmp(args[0], "echo") == 0) {
        // if there's no argument after echo, well just print dat newline
        if (args[1] == NULL) {
            printf("\n");
            return 1;
            // we have something to print
        } else {
            // start at 1 because args[0] is the echo command
            int i = 1;
            // print each argument until we reach the end.
            while (args[i] != NULL) {
                printf("%s ", args[i]);
                i++;
            }
            // new line so it places current directory below.
            printf("\n");
            return 1;

        }
    }
    return 0;
}
