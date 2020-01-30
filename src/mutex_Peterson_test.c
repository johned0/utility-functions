// Peterson’s two process mutual exclusion example
// (C) 2020 John Edwards
// Cygwin : gcc mutex_Peterson_test.c -lpthread -o mutex_Peterson_test.exe
// Linux  : gcc mutex_Peterson_test.c -lpthread -o mutex_Peterson_test

// This mutex does not support resource request while a current request is pending
// If this scenario is a possibility then use mutex_peterson_interfaceN_trying() to ensure there is no pending request

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mutex.h"

h_Mutex myMutex;

char globalStringResource [80];                         // String to pass message from thread #0 to thread #1


void* producer_thread(void* arg)
{
    mutex_peterson_p1_request(&myMutex);

    printf("Producer thread started\n");

    sprintf(globalStringResource, "Hello From Producer Thread");
    printf("Producer thread sent message via global resource\n");

    for (unsigned i = 0; i < (0x3FFFFFFF); i++)         // Wait
        ;

    printf("Producer thread finished\n");

    mutex_peterson_p1_release(&myMutex);

    return NULL;
}

void* consumer_thread(void* arg)
{
    mutex_peterson_p2_request(&myMutex);

    printf("Consumer thread started\n");

    printf("Consumer thread, message received : %s\n", globalStringResource);

    for (unsigned i = 0; i < (0x3FFFFFFF); i++)         // Wait
        ;

    printf("Consumer thread finished\n");

    mutex_peterson_p2_release(&myMutex);

    return NULL;
}

int main(void)
{
    pthread_t tid[2];

    mutex_peterson_init (&myMutex);                     // Initialize the mutex

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

