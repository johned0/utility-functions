// Circular buffer for integers
// (C) 2020 John Edwards

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIRCULAR_BUFFER_SIZE    8           // Maximum size of buffer


char circularBuffer[CIRCULAR_BUFFER_SIZE];  // Circular buffer

int readIndex   = 0;                        // Data read index
int writeIndex  = 0;                        // Data write index
int bufferDepth = 0;                        // Circular buffer depth

int cb_read (int *it)
{
    int successFlag = 0;

    if (bufferDepth != 0) {                         // Check if circular buffer NOT empty
        *it = circularBuffer[readIndex];             // Read value from circular buffer

        bufferDepth--;                              // Decrease circular buffer depth
        readIndex++;                                // Increment read index

        if (readIndex == CIRCULAR_BUFFER_SIZE) {    // Loop if we are at the end of the array
            readIndex = 0;
        }
        successFlag = 1;
    }
    return (successFlag);
}

int cb_write (int it)
{
    int successFlag = 0;

    if (bufferDepth != CIRCULAR_BUFFER_SIZE) {      // Check if circular buffer NOT full
        circularBuffer[writeIndex]= it;             // Write value to circular buffer

        bufferDepth++;                              // Increase circular buffer depth
        writeIndex++;                               // Increment write index

        if (writeIndex == CIRCULAR_BUFFER_SIZE) {   // Loop if we are at the end of the array
            writeIndex = 0;
        }
        successFlag = 1;
    }
    return (successFlag);
}


int main(void)
{
    char inputString[20];
    int  bufferedInt;

    while (1) {
        printf ("Enter an integer to push to circular buffer (max length = %d), \"r\" to read a value, \"x\" to exit\n", CIRCULAR_BUFFER_SIZE);

        scanf ("%s", &inputString);

        if (strcmp (inputString, "x") == 0) {       // Exit
            exit(0);
        }

        else if (strcmp (inputString, "r") == 0) {  // Read from circular buffer
            if (cb_read (&bufferedInt) == 0) {
                printf("Circular buffer empty!\n");
            }
            else {
                printf("String : %d\n", bufferedInt);
            }
        }

        else {                                      // Write to circular buffer
            if (cb_write (atoi(inputString)) == 0) {
                printf("Circular buffer full!\n");
            }
            else {
                printf ("String written to circular buffer\n");
            }
        }
    }
}

