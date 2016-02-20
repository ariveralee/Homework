#include "myheader.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define TOKEN_BUFFSIZE 64
#define LINE_BUFFSIZE 1024
#define TOKEN_DELIMITERS " \t\r\n\a"

int main(int argc, char **argv) {

  // calls the shell funcion to loop
  shell_loop();


  return EXIT_SUCCESS;

}

void  shell_loop() {

  // we need to read commands from the standard input
  char  *readLine;
  // this is the arguments
  char  **args;
  int   status;

  do {
    // this prints the carrot so we know to await input
    printf("shell shell give me input >" );
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

char *read_line() {
  char *line;
// allocates space for the line
  size_t buf = 0;
// getline takes the string from line and reads up to buf size from stdin
  getline(&line, &buf, stdin);
  return line;
}

char **read_args(char *line) {
  int bufferSize = LINE_BUFFSIZE;
  int index = 0;
  char **tokens = malloc(bufferSize * sizeof(char*));
  char *token;

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

/* This method launches commands and processes */
int launcher(char **args) {

  pid_t pid;
  pid_t wpid;
  int status;

// we get a -1 if the fork fails
  if ((pid = fork()) == -1) {
    print_failure();
  }
// if we have a 0, we are in the child process
  if (pid == 0) {
// if we have get a -1 there was a failure for the execvp
    if (execvp(args[0], args) == -1) {
      print_failure();
    }
  } else {
    // we are in the parent process
    do {
      // WUNTRACED reports back to this process the status of the child processes
      wpid = waitpid(pid, &status, WUNTRACED);
      // no non zero value means that the child process has not been terminated for WIFEXITED
      // WIFSIGNALED, if we get non zero value back, this means child process was terminated
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

void print_failure() {
  printf("Command Not Found\n");
  exit(EXIT_FAILURE);

}






















