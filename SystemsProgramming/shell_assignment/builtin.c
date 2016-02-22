#include "myheader.h"

#define CMD_BUFFSIZE 256
/********************************************************************************/
/* The below arrays are essential for us listing our commands for the builtin
 * functions as well as arguments. We also store the array of pointers to our
 * builtin functions
 */
/********************************************************************************/


/* make a list of built in commands */
char *builtin_cmd[] = {
    "help",
    "cd",
    "exit",
    "clear",
    "echo",
    "environ",
    "ls"
};

/* make an array of function pointers what take an array of strings and return int */
int (*builtin_function[]) (char **) = {
    &myshell_help,
    &myshell_cd,
    &myshell_exit,
    &myshell_clr,
    &myshell_echo,
    &myshell_env,
    &myshell_ls
};

/* array of options to use with ls as arg[1] */
char *ls_options[] = {
    "-l",
    "-f",
    "-m",
    "-a",
    "-al",
    "-g",
    "-d",
    "-o",
    "-r",
    "-s",
    "-t",
    "-x",
    "-1"
};
/********************************************************************************/
/* All builtin functions for the running of this program is listed below */
/********************************************************************************/

/**
 * This function is for the changing of directories by the user. If
 * we get the "HOME" environment in case the user uses the cd command
 * without any addtionial arguments.
 */
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

/**
 * this function prints the help menu for users to learn more about
 * about the uses of functions or for correct commands
 */
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

/* *
 *simple function, if the user calls exit, we exit
 */
int myshell_exit(char **args) {
    return 0;
}

/**
 *this function is for the executing of the builtin commands.
 * if there's nothing enter, there's no commands so we do nothing
 * else, there is a command so we compare it to the list of builtin
 * commands that we have. If there's a match then we call that builtin
 * command. However, if there is not a match, we assume we need to launch
 * something else so we pass these arguments to the launcher.
 */
int execute_cmd(char **args) {
    int     i;
    int     j = 0;
    int     k = 0;
    char    *redirect_arg[256];
    int     alt;
    int     runBackground = 0;
    int     specialCmd = 0;

// if we have NULL, there's no command entered, do nothing.
    if (args[0] == NULL) {
        printf("\n");
        return 1;
    }

// before we do anything, let's check to see if we have special commands
    while (args[j] != NULL) {
        if ((strcmp(args[j], ">") == 0) || (strcmp(args[j], "&") == 0) || (strcmp(args[j], "<") == 0)) {
            specialCmd = 1;
            break;
        }
        redirect_arg[j] = args[j];
        j++;
    }
    if (specialCmd == 0) {
        // we check to see if the first argument matches our builtin commands
        for (i = 0; i < number_builtin(); i++) {
            if (strcmp(args[0], builtin_cmd[i]) == 0) {
                // if it does, we run that builtin function
                return (*builtin_function[i])(args);
            }
        }
    }

// if we get to here, none of the builtin functions were used so it must be a program.
    while (args[k] != NULL && runBackground == 0) {
        if (strcmp(args[k], "&") == 0) {
            runBackground = 1;
        }
        // check to see if we have a pipe
        else if (strcmp(args[k], "|") == 0) {
            pied_piper(args);
            return 1;

            // if there's < detected, we have I/O redirection BUT we need to make
            // sure that the format is correct.
        } else if (strcmp(args[k], "<") == 0) {
            alt = k + 1;
            if (args[alt] == NULL || args[alt + 1] == NULL || args[alt + 2] == NULL) {
                printf("You must provide enough arguments! see help for more info \n");
                return -1;
            } else {
                if (strcmp(args[alt + 1], ">") != 0) {
                    printf("Usage: expected '>' but found %s\n", args[alt + 1]);
                    return -2;
                }
            }
            shell_IO(redirect_arg, args[k + 1], args[k + 3], 1);
            return 1;
        }
        // if we have >, we have output redirection, make sure it's correct format
        else if (strcmp(args[k], ">") == 0) {
            if (args[k + 1] == NULL) {
                printf("You must provide enough arguments! see help for more info\n");
                return -1;
            }
            shell_IO(redirect_arg, NULL, args[k + 1], 0);
            return 1;
        }
        k++;
    }
    // run the program or arguments
    launcher(redirect_arg, runBackground);
    return 1;
}

/**
 *This function is for when the user has entered clear, to cheat we
 * use the system call "clear"
 */
int myshell_clr(char **args) {
    if (strcmp(args[0], "clear") == 0) {
        system("clear");
    }
    return 1;
}

/* *
 * This function is my implementation of echo. If the first argument
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

/* this function allows for the user to enter the command
 * environ, which is done utilizing a system call
 */
int myshell_env(char **args) {
    if (strcmp(args[0], "environ") == 0) {
        system("env");
        return 1;
    }
    return 0;
}

/* *
 * this function utilizes the ls command by using a system call
 * for ls. This also allows for the user to use several ls
 * arguments for more customization of the ls command.
 */
int myshell_ls(char **args) {
    // if the first argument is ls
    if (strcmp(args[0], "ls") == 0) {
        // if the second argument is null
        if (args[1] == NULL) {
            // dat system call
            system("ls");
            return 1;
            // else, we have a second argument for ls, get it.
        } else {
            int i;
            for (i = 0; i < number_ls(); i++) {
                if (strcmp(args[1], ls_options[i]) == 0) {
                    // make a char array with space for addtional arg
                    char ls_cmd[10] = "ls ";
                    // concatenate the second arg with ls_cmd
                    strcat(ls_cmd, ls_options[i]);
                    // call the system arg, returns -1 on failure
                    if (system(ls_cmd) == -1) {
                        printf("Error calling ls command with 2nd arg\n");
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }

        }
    }
    return 0;
}

/**********************************************************************************/
/**
 * The function below handles the redirecting associated with input and output
 * the design of this function has been heavily influence by Professor Fiore's
 * examples in class
 */
int shell_IO(char *args[], char *inputFile, char *outputFile, int options) {
    int fileDesc;
    int i ;
    pid_t pid;
    // fork returns -1 on failure.
    if ((pid = fork()) == -1) {
        printf("child process creation failed\n");
        return 0;
    }

    // we are in the child process
    if (pid == 0) {
        // option 0 is for output redirection
        if (options == 0) {
            // if the file does not exist, create it and trunicate at 0, write only.
            fileDesc = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
            //replace dat standard output with the file.
            dup2(fileDesc, STDOUT_FILENO);
            close(fileDesc);
            // option 1 is for input and output redirection.
        } else if (options == 1) {
            //open a file for reading only.
            fileDesc = open(inputFile, O_RDONLY, 0600);
            // replace standard input with our file
            dup2(fileDesc, STDIN_FILENO);
            close(fileDesc);
            // now we need to do the same thing for the output file
            fileDesc = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
            dup2(fileDesc, STDOUT_FILENO);
            close(fileDesc);
        }
        // we need to set the cwd to the parent so we can come back
        setenv("parent", getcwd(currentDir, 1024), 1);

        // we check to see if the first argument matches our builtin commands
        for (i = 0; i < number_builtin(); i++) {
            if (strcmp(args[0], builtin_cmd[i]) == 0) {
                // if it does, we run that builtin function
                return (*builtin_function[i])(args);
            }
        }
        // //now lets launch the argument if it fails, exit out.
        // if (execvp(args[0], args) == -1) {
        //     printf("error for I/O launch\n");
        //     return;
        // }
    }
    return 0;
    //we wait for this to finish.
    waitpid(pid, NULL, 0);
}

/**
 * This function handles piping. we work to see how many commands we have
 * for piping. After this we work to execute the commands between the pipes
 * utilizing the pipe call , fork call and execvp calls At the end, we close
 * the file descriptors and wait for the program to finish */
void pied_piper(char *args[]) {
    // These are the file descriptors
    int     fileDesc[2];
    int     fileDesc2[2];
    char    *cmd[CMD_BUFFSIZE];
    pid_t   pid;
    int     cmd_num = 0;
    int     endOfCmd = 0;

    //while this is inefficient, it works. counter variables for various loops
    int i = 0;
    int j = 0;
    int n = 0;
    int m = 0;

    // we know there's commands for the pipes, lets see how many there are
    while (args[m] != NULL) {
        // look for pipes, each one tells us there's a command
        if (strcmp(args[m], "|") == 0) {
            // increment the commands
            cmd_num++;
        }
        m++;
    }
    cmd_num++;


// this loop will execute each command between the | with the standard I/O being routed
    while (args[j] != NULL && endOfCmd != 1) {
        n = 0;
        // store the commands to be executed
        while (strcmp(args[j], "|") != 0) {
            cmd[n] = args[j];
            j++;
            // if args[j] == NULL, we have no more commands, set the flag
            if (args[j] == NULL) {
                endOfCmd = 1;
                n++;
                break;
            }
            n++;
        }
        // at thispoint, the last command will be NULL to so we know the end of args
        cmd[n] = NULL;
        j++;


        // check to see what iteration we are in
        if (i % 2 == 0) {
            pipe(fileDesc2);
        } else {
            pipe(fileDesc);
        }

        // now we will fork to create a process for the cmd
        //if there's an error, we close the file descriptors and return
        if ((pid = fork()) == -1) {
            if (i != cmd_num - 1) {
                if (i % 2 != 0) {
                    close(fileDesc[1]);
                } else {
                    close(fileDesc2[1]);
                }
            }
            printf("error creating child process in pipe\n");
            return;
        }

        // if it's equal to zero we know it's the child process
        if (pid == 0) {
            // do the first command
            if (i == 0) {
                dup2(fileDesc2[1], STDOUT_FILENO);
            }
            /* if we have reached the last command, we will replace the stdin
             * for each pipe.*/
            else if (i == cmd_num - 1) {
                if (cmd_num % 2 == 0) {
                    dup2(fileDesc2[0], STDIN_FILENO);
                } else {
                    dup2(fileDesc[0], STDIN_FILENO);
                }
                // if we reach this point there's two pipes because there's a cmd in the middle
            } else {
                if (i % 2 != 0) {
                    dup2(fileDesc2[0], STDIN_FILENO);
                    dup2(fileDesc[1], STDOUT_FILENO);
                } else {
                    dup2(fileDesc[0], STDIN_FILENO);
                    dup2(fileDesc2[1], STDOUT_FILENO);
                }
            }
            if ( execvp(cmd[0], cmd) == -1) {
                printf("Error executing command in pipe\n");
                return;
            }
        }

        // we must close the descriptors for the parent
        if (i == 0) {
            close(fileDesc2[1]);
        }
        else if (i == cmd_num - 1) {
            if (cmd_num % 2 != 0) {
                close(fileDesc[0]);
            } else {
                close(fileDesc2[0]);
            }
        } else {
            if (i % 2 != 0) {
                close(fileDesc2[0]);
                close(fileDesc[1]);
            } else {
                close(fileDesc[0]);
                close(fileDesc2[1]);
            }
        }
        // wait for it to finish.
        waitpid(pid, NULL, 0);
        i++;
    }
}

/********************************************************************************/
/* The functions listed below are not core functions, but rather helper
 * functions to assist the functions to run properly.
 */
/********************************************************************************/

/* this function returns the size of the builtin functions array */
int number_builtin() {
    return ((sizeof(builtin_cmd)) / (sizeof(char *)));
}

/* this function returns the size of the ls arguments array */
int number_ls() {
    return ((sizeof(ls_options)) / (sizeof(char *)));
}
