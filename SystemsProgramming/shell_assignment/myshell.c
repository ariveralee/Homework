#include "myheader.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define TOKEN_BUFFSIZE 64
#define LINE_BUFFSIZE 1024
#define TOKEN_DELIMITERS " \t\r\n\a"

int main(int argc, char **argv) {

  // calls the shell funcion to loop
  shell_prompt();


  return EXIT_SUCCESS;

}
/* *
 * This function is for the shell prompt. The program enters
 * with this prompt that allows the user to enter in commands
 * the function will not terminate until ctr-c or exit has
 * been requested
 */
void  shell_prompt() {

  // we need to read commands from the standard input
  char  *readLine;
  // this is the arguments
  char  **args;
  int   status;
  char  hostname[1024] = "";
// making space.
  currentDir = (char*) calloc(1024, sizeof(char));
  do {
    // this prints the carrot so we know to await input with the current directory
    gethostname(hostname, sizeof(hostname));
    printf("%s@%s %s >\n", getenv("LOGNAME"), hostname, getcwd(currentDir, 1024));

    // read the contents from the line
    readLine = read_line();
    // need to tokenize the arguments so we know what is going on
    args = read_args(readLine);
    // based on this we know what commands to run
    status = execute_cmd(args);

    //need to free everything so don't have memory leaks
    free(readLine);
    free(args);
  } while (1);
}

/**
 * This function reads the line entered by the user and 
 * returns it to the  calling function.
 */
char *read_line() {
  char   *line;
// allocates space for the line
  size_t  buf = 0;
// getline takes the string from line and reads up to buf size from stdin
  getline(&line, &buf, stdin);
  return line;
}

/**
 * This function has the simple job of reading the arguments from our cmd
 * line. Once the commands have been read, we return tokens for the args.
 */
char **read_args(char *line) {
  int   bufferSize = LINE_BUFFSIZE;
  int   index = 0;
  char  **tokens = malloc(bufferSize * sizeof(char*));
  char  *token;

// if we could not allocated space for whatever reason, we need to quit out.
  if (!tokens) {
    print_failure();
  }
// tokenize the line based on the delimters that i defined previously
  token = strtok(line, TOKEN_DELIMITERS);
  while (token != NULL) {
    tokens[index] = token;
    index++;
    if (index >= bufferSize) {
      // if there is not enough space, we add addtional to take the input
      bufferSize += LINE_BUFFSIZE;
      tokens = realloc(tokens, bufferSize * sizeof(char*));
      if (!tokens) {
        print_failure();
      }
    }
// if we have reached this point then there are no more tokens to grab.
    token = strtok(NULL, TOKEN_DELIMITERS);
  }
// we have an array of tokens that we have grabbed, now we need to null terminate it.
  tokens[index] = NULL;
  // return the tokens so we can run the command
  return tokens;
}

/**
 * This method launches commands and process if they are not built in
 * to the shell. the parameter runBackground is to see if the user has
 * entered & at the end of the command, if runBackground is 0, we proceed
 * as normal, if not we go right back to the command line and let the
 * program run in the background.
 */
void launcher(char **args, int runBackground) {

  pid_t pid;
  pid_t wpid;
  int   status;

// we get a -1 if the fork fails
  if ((pid = fork()) == -1) {
    printf("child process creation failed\n");
    return;
  }
// if we have a 0, we are in the child process
  if (pid == 0) {
    // set the parent path name as environ variable for the child
    setenv("parent", getcwd(currentDir, 1024), 1);
// if we have get a -1 there was a failure for the execvp
    if (execvp(args[0], args) == -1) {
      printf("-cshell: %s: Command not found!\n", args[0]);
    }
  } else {
    // we are in the parent process
    if (runBackground == 0) {
      do {
        // WUNTRACED reports back to this process the status of the child processes
        wpid = waitpid(pid, &status, WUNTRACED);
        // no non zero value means that the child process has not been terminated for WIFEXITED
        // WIFSIGNALED, if we get non zero value back, this means child process was terminated
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
      // do nothing we wait.
    }
  }
}



void print_failure() {
  printf("ERROR!\n");
  exit(EXIT_FAILURE);
}
