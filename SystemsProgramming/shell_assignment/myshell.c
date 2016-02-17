#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define TOKEN_BUFFSIZE 64
#define TOKEN_DELIMITERS " \t\r\n\a"

/* Function Declarations */
void shell_loop();
char *read_line();
char **read_args(char *);

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
    //status = run_execute(args);

    //need to free everything so don't have memory leaks
    free(readLine);
    free(args);
  } while (1);



}

char *read_line() {
  char *line;
// allocates space for the line
  ssize_t buf = 0;
// getline takes the string from line and reads up to buf size from stdin
  getline(&line, &buf, stdin);
  return line;
}

char **read_args(char *line) {
  int bufferSize = TOKEN_BUFFSIZE;
  int index = 0;
  char **tokens = malloc(bufferSize * sizeof(char*));
  char *token;

// if we could not allocated space for whatever reason, we need to quit out.
  if (!tokens) {
    fprintf(stderr, "shelly: there's been some sort of allocation error :'(\n");
    exit(EXIT_FAILURE);
  }
// tokenize the line based on the delimters that i defined previously
  token = strtok(line, TOKEN_DELIMITERS);
  while (token != NULL) {
    tokens[index] = token;
    index++;
    if (index >= bufferSize) {
      tokens = realloc(tokens, bufferSize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "shelly: hmmm there seems to have been an allocation error.\n");
        exit(EXIT_FAILURE);
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

