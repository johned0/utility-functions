// Circular buffer for strings

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIRCULAR_BUFFER_SIZE    8           // Maximum size of buffer
#define MAX_STRING_LENGTH       20          // Maximum length of any individual string


char circularBuffer[CIRCULAR_BUFFER_SIZE*MAX_STRING_LENGTH];   // Circular buffer

int readIndex   = 0;                        // Data read index
int writeIndex  = 0;                        // Data write index
int bufferDepth = 0;                        // Circular buffer depth

int cb_read (char *st)
{
    int successFlag = 0;

    if (bufferDepth != 0) {                         // Check if circular buffer NOT empty
        memcpy (st, &circularBuffer[readIndex*MAX_STRING_LENGTH], MAX_STRING_LENGTH);       // Read string from circular buffer

        bufferDepth--;                              // Decrease circular buffer depth
        readIndex++;                                // Increment read index

        if (readIndex == CIRCULAR_BUFFER_SIZE) {    // Loop if we are at the end of the array
            readIndex = 0;
        }
        successFlag = 1;
    }
    return (successFlag);
}

int cb_write (char *st)
{
    int successFlag = 0;

    if (bufferDepth != CIRCULAR_BUFFER_SIZE) {      // Check if circular buffer NOT full
        memcpy (&circularBuffer[writeIndex*MAX_STRING_LENGTH], st, MAX_STRING_LENGTH);      // Write string to circular buffer

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
    char inputString[MAX_STRING_LENGTH];
    char bufferedString[MAX_STRING_LENGTH];

    while (1) {
        printf ("Enter a sting to push to circular buffer (max length = %d), \"r\" to read a value, \"x\" to exit\n", CIRCULAR_BUFFER_SIZE);

        scanf ("%s", &inputString);

        if (strcmp (inputString, "x") == 0) {       // Exit
            exit(0);
        }

        else if (strcmp (inputString, "r") == 0) {  // Read from circular buffer
            if (cb_read (bufferedString) == 0) {
                printf("Circular buffer empty!\n");
            }
            else {
                printf("String : %s\n", bufferedString);
            }
        }

        else {                                      // Write to circular buffer
            if (cb_write (inputString) == 0) {
                printf("Circular buffer full!\n");
            }
            else {
                printf ("String written to circular buffer\n");
            }
        }
    }
}

