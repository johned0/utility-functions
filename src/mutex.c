// Peterson’s two process mutual exclusion example
// Cygwin : gcc mutex.c -lpthread -o mutex.exe
// Linux  : gcc mutex.c -lpthread -o mutex

// This mutex does not support resource request while a current request is pending
// If this scenario is a possibility then use mutex_interfaceN_pending() to ensure there is no pending request

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define False   0
#define True    1


struct Mutex_t
{
    int Trying1;
    int Trying2;
    int Turn;
};

void mutex_init (struct Mutex_t *mutex)             // Initialize the mutex
{
    mutex->Trying1 = False;
    mutex->Trying2 = False;
    mutex->Turn = 0;
}

                                                    // Interface 1
void mutex_p1_request (struct Mutex_t *mutex)       // Request access to mutex
{
    mutex->Trying1 = True;
    mutex->Turn = 1;

    while ((mutex->Trying2 == True) && (mutex->Turn == 1))  // Wait until (turn == me) OR other process not trying
        ;
}

int mutex_p1_pending (struct Mutex_t *mutex)       // Return pending status
{
    return (mutex->Trying1);
}

void mutex_p1_release (struct Mutex_t *mutex)       // Release access to mutex
{
    mutex->Trying1 = False;                         // Release the mutex
}

                                                    // Interface 2
void mutex_p2_request (struct Mutex_t *mutex)       // Request access to mutex
{
    mutex->Trying2 = True;
    mutex->Turn = 2;

    while ((mutex->Trying1 == True) && (mutex->Turn == 2))  // Wait until (turn == me) OR other process not trying
        ;
}

int mutex_p2_pending (struct Mutex_t *mutex)       // Return pending status
{
    return (mutex->Trying2);
}

void mutex_p2_release (struct Mutex_t *mutex)       // Release access to mutex
{
    mutex->Trying2 = False;                         // Release the mutex
}


struct Mutex_t myMutex;

char globalStringResource [80];                     // String to pass message from thread #0 to thread #1


void* producer_thread(void* arg)
{
    mutex_p1_request(&myMutex);

    printf("Producer thread started\n");

    sprintf(globalStringResource, "Hello From Producer Thread");
    printf("Producer thread sent message via global resource\n");

    for (unsigned i = 0; i < (0x3FFFFFFF); i++)     // Wait
        ;

    printf("Producer thread finished\n");

    mutex_p1_release(&myMutex);

    return NULL;
}

void* consumer_thread(void* arg)
{
    mutex_p2_request(&myMutex);

    printf("Consumer thread started\n");

    printf("Consumer thread, message received : %s\n", globalStringResource);

    for (unsigned i = 0; i < (0x3FFFFFFF); i++)     // Wait
        ;

    printf("Consumer thread finished\n");

    mutex_p2_release(&myMutex);

    return NULL;
}

int main(void)
{
    pthread_t tid[2];

    mutex_init (&myMutex);              // Initialize the mutex

    int err = pthread_create(&(tid[0]),
                             NULL,
                             &producer_thread, NULL);
    if (err != 0)
        printf("Producer thread creation error :[%s]", strerror(err));

    err = pthread_create(&(tid[1]),
                             NULL,
                             &consumer_thread, NULL);
    if (err != 0)
        printf("Consumer thread creation error :[%s]", strerror(err));


    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}

