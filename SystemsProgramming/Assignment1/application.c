#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define MY_FILE "output.txt"
#define FILE_OPEN_ERROR "Could not open or create the file!\n"

#define RECORD_LEN 10
#define LOOP_TIME 100000
#define SEQUENCE_LEN 120

void fileWriter(FILE *);
void compareFile(char[][121], FILE *);
struct  timeval end;

/* This method creates a file and writes 10 records to the file.
 * Each record is a sequence of 120 random characters. These
 * sequences are stored in a array for future comparison.
 * Upon completion of writing to the file, the compareFile()
 * method is called.
 */
void fileWriter(FILE *file_pointer) {

    char sequence       [SEQUENCE_LEN + 1];
    char sequenceHolder [RECORD_LEN] [SEQUENCE_LEN + 1];

    // loop for number of times we repeat the below (To keep the program running)
    for (int i = 0; i < LOOP_TIME; i++) {
        // seed the random generator adding i ensures distinct sequences
        srand(time(NULL) + i);
        rand();
        // Loop for keeping track of the number of records we write
        for (int j = 0; j < RECORD_LEN; j++) {

            // Loop for keeping track of the number of chars we write
            for (int k = 0; k < SEQUENCE_LEN; k++) {

                //The highest remainder is 25 ( 25+65 is 90 or 'Z') (only upper char)
                sequence[k] = ((rand() % 26) + 65);
            }
            // Add the null character to end the sequence of characters
            sequence[120] = '\0';

            // Store our sequence into an array for comparison later.
            strcpy(sequenceHolder[j], sequence);

            // Write the 120 characters to the file and separate by a newline.
            fputs(sequence, file_pointer);
            fputs("\n", file_pointer);
        }
        // rewind to the begining of the file before reading from it.
        rewind(file_pointer);
        // Reads and compares the file to the sequences we have stored
        compareFile(sequenceHolder, file_pointer);
    }
}

/* This method randomly selects a record that we have stored (that was
 * written to the file) and compares it to what we read from the file.
 * This file is compared and prints a statement only if no matches are found.
*/
void compareFile(char sequenceHolder[][SEQUENCE_LEN + 1], FILE *file_pointer) {
    // counter to keep track of number of incorrect reads, if it's 10, we got problems.
    int     count = 0;
    // We need to randomly select an index from from our 10 records
    int     randFile = (rand() % RECORD_LEN);
    // String array to store read lines from file +2 to account for the '\0' and \n chars
    char    fileInput[RECORD_LEN][SEQUENCE_LEN + 2];

    // There's 10 records, so 10 lines, let's grab each line
    for (int i = 0; i < RECORD_LEN; i++) {
        fgets(fileInput[i], SEQUENCE_LEN + 2, file_pointer);

        // fgets grabs the \n char as well, in order to properly compare, we must remove it
        fileInput[i][strlen(fileInput[i]) - 1] = '\0';

        // compare the string read from the file to what we have on record
        if (strcmp(sequenceHolder[randFile], fileInput[i]) != 0) {
            count++;
        }
        // if we reach 10, that means none of the records match the file, Boo boo.
        if (count == 10) {
            printf("Found no matches");
        }
    }
    // Need to rewind to the begining to overwrite the file.
    rewind(file_pointer);
}

int main() {
    gettimeofday(&end, NULL);
    double endTime = ((end.tv_sec + end.tv_usec) / 1000000.0);
    int p = (int)getpid();

    // grabs the time we reach the file
    printf(" PID: %d Reached the App %f\n", p, endTime);
    FILE    *file_pointer; // File pointer variable to write to the file.
    char    pid[10]; // char array to store pid string
    char    fileName[SEQUENCE_LEN]; // holds the string for the file name.


    //Takes the output of getpid and puts it into char array pid
    snprintf(pid, 10, "%d", (int)getpid());

    // Copy the pid to filename, this allows us to have a unique file name
    strcpy(fileName, pid);

    //add to have a path to create the file.
    strcat(fileName, MY_FILE);

    // Open the file, w+ allows for reading and writing.
    file_pointer = fopen(fileName, "w+");

    // If the filePointer is not null
    if (file_pointer) {
        fileWriter(file_pointer);
    } else {
        printf(FILE_OPEN_ERROR);
    }

    // Close the file and Delete the file before the program exits.
    fclose(file_pointer);
    remove(fileName);
    return 1;
}








