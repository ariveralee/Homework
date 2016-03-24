#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define PRODUCER_SPEED      50000      // producer thread sleep time micro secs  
#define CONSUMER_SPEED      80000      // consumer thread sleep time micro secs 
#define NUM_PRODUCERS       4        // number of producers 
#define NUM_CONSUMERS       5          // number of consumers 
#define BUFFER_SIZE         25         //  number of slots in buffer 
#define MAX                 50        // number of items produced/consumed 
#define PRINT_LOG           0          // 1 for use log printing and 0 for console


void closeLog();
void writeLog();

// struct to create our buffer
typedef struct {
    int buf[BUFFER_SIZE];       // the buffer
    size_t len;                 // number of items in the buffer
    pthread_mutex_t mutex;      // needed to add/remove data from the buffer
    pthread_cond_t can_produce; // this is signaled once items are removed
    pthread_cond_t can_consume; // signaled once items have been added
} buffer_t;

// creates a new buffer, to test to see if producer waiting happens
// try setting the length to or near the MAX limit
buffer_t buffer = {
    .len = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .can_produce = PTHREAD_COND_INITIALIZER,
    .can_consume = PTHREAD_COND_INITIALIZER
};

int producers_done = 0;
int consumers_done = 0;
// global file pointer for writing to a "log"
FILE    *file_pointer;
FILE    *saved;
char    fileName[] = "log.txt";


/**
 * The producer produces items in the buffer if and only if there is
 * room to do so. If the buffer is empty, then the producer will insert
 * items. In this case, the items being inserted are random integers.
 **/
void* producer(void *arg) {
    //buffer_t *buffer = (buffer_t*)arg;
    int i = 0;
    int id = *((int *) arg);
    while (1) {
#ifdef UNDERFLOW
        // used to show that if the producer is somewhat "slow"
        // the consumer will not fail (i.e. it'll just wait
        // for new items to consume)
        usleep(PRODUCER_SPEED);
#endif
        // locks the buffer
        pthread_mutex_lock(&buffer.mutex);

        // if the buffer len is equal to the buffer size, we are full
        if (buffer.len == BUFFER_SIZE) {
            writeLog();
            // lets wait until an item has been consumed, then we can produce.
            printf("Producer %d: Buffer is full, cannot insert any items \n", id);
            closeLog();
            pthread_cond_wait(&buffer.can_produce, &buffer.mutex);
            // if there's no room, we need to let go control of the lock
            pthread_mutex_unlock(&buffer.mutex);

        } else {
            //random number to input.
            int produced = rand();
            writeLog();
            printf("Producer %d Produced: %d\n", id , produced);
            closeLog();
            // add the item to the buffer
            buffer.buf[buffer.len] = produced;
            // increment the buffers length.
            ++buffer.len;

            // if we get here, then the consumer can, well... consume.
            pthread_cond_signal(&buffer.can_consume);
            pthread_mutex_unlock(&buffer.mutex);
            i++;
            // if we reached the max number of items, then there's
            // nothing to produce.
            if (i == MAX) {
                printf("DONE in Producer %d\n", id);
                producers_done++;
                return NULL;
            }
        }
    }

    // we should never reach here, i hope
    return NULL;
}

/**
 * The consumer consumes items in the buffer if and only if there is
 * something contained within it. If the buffer becomes empty, then
 * there will be no more consumption until there a new item has been produced
 **/
void* consumer(void *arg) {
    int i = 0;
    int id = *((int *) arg);
    while (1) {
#ifdef OVERFLOW
        // show that the buffer won't overflow if the consumer
        // runs too slow.
        usleep(CONSUMER_SPEED);
#endif
        // make sure no one else can get in.
        pthread_mutex_lock(&buffer.mutex);

        // if the buffer length is 0, then we know it's empty and we need to wait
        if (buffer.len == 0) { // empty
            // if all the producers have finished, then we are done.
                writeLog();
                // we know all of the producers are not done, then we can wait
                printf("Consumer %d: Buffer is empty waiting for items\n", id);
                closeLog();
                pthread_cond_wait(&buffer.can_consume, &buffer.mutex);
                // if there's nothing to consume, we need to let go of the lock
                pthread_mutex_unlock(&buffer.mutex);
            
        } else {

            // if we get here, then it's time to consume, yum.
            --buffer.len;
            writeLog();
            printf("Consumer %d Consumed: %d\n", id , buffer.buf[buffer.len]);
            closeLog();
            // now new items can be produced, under the assumption that there's space
            pthread_cond_signal(&buffer.can_produce);
            pthread_mutex_unlock(&buffer.mutex);
            i++;
            // if we reach the max, then we have had our fill of consuming
            if (i == MAX) {
                printf("DONE in Consumer %d\n", id);
                consumers_done++;
                // just break out, there's nothing more to do.
                break;
            }
        }
    }
    // never reached
    return NULL;
}

int main(int argc, char *argv[]) {
    // counter variable
    int     i;
    // reference so we associate the id with threads
    int     *idRef;
    // to return
    void    *ret;


    // thread array for our threads
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    // creating the producers
    for (i = 0; i < NUM_PRODUCERS; i++) {
        idRef = (int *) malloc(sizeof(int));
        *idRef = i;
        pthread_create(producers + i, NULL, producer, (void*) idRef);
    }

    // creating the consumers
    for (i = 0; i < NUM_CONSUMERS; i++) {
        idRef = (int *) malloc(sizeof(int));
        *idRef = i;
        pthread_create(consumers + i, NULL, consumer, (void*) idRef);
    }
    // joining the producers
    for (i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(*(producers + i), &ret); // will wait forever
    }
    // joining the consumers
    for (i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(*(consumers + i), &ret);
    }

    // kill everything
    exit(0);
}


/**
 * This function is for changing the stdout to a file and appending it
 * for when we want to see the output of the program in a log file
 */
void writeLog() {
    if (PRINT_LOG == 1) {
        saved = stdout;
        stdout = fopen(fileName, "a");
    } else {
        return;
    }
}
/**
 * This makes sure that we change stdout to the console
 * from the the file
 */
void closeLog() {
    if (PRINT_LOG == 1) {
        fclose(stdout);
        stdout = saved;
    } else {
        return;
    }

}