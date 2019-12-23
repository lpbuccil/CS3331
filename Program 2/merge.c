/* ----------------------------------------------------------- 
* NAME : Lucas Buccilli                         User ID: lpbuccil 
* DUE DATE : 10/18/2019                                       
* PROGRAM ASSIGNMENT 2                                        
* FILE NAME : merge.c (your unix file name)            
* PROGRAM PURPOSE :                                           
*    Implementation of a current merge sort       
* ----------------------------------------------------------- */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFERSIZE 1000

/* ----------------------------------------------------------- 
* FUNCTION  _strdup : (function name)                          
*     Copy the contents of a string and store them into memory                          
* PARAMETER USAGE :                                           
*    *str  the string to be copied            
* FUNCTION CALLED :                                           
*    malloc, memcpy, strlen         
* ----------------------------------------------------------- */
char *_strdup(const char *str)
{
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup)
        memcpy(dup, str, len);
    return dup;
}

/* ----------------------------------------------------------- 
* FUNCTION  print : (function name)                          
*     Wrapper for printing a buffer to stdout                         
* PARAMETER USAGE :                                           
*    buffer string to be printed          
* FUNCTION CALLED :                                           
*    write, memset     
* ----------------------------------------------------------- */
void print(char buff[BUFFERSIZE])
{
    write(1, buff, strlen(buff));
    memset(buff, 0, BUFFERSIZE);
}

/* ----------------------------------------------------------- 
* FUNCTION  recursiveBinary : (function name)                          
*     WUsed to find the index of where a element belongs in an array                        
* PARAMETER USAGE :                                           
*    value to find index for, low start index, high last index, array of values to look through
* FUNCTION CALLED :                                           
*    recursiveBinary
* ----------------------------------------------------------- */
int recursiveBinary(int value, int low, int high, int *array)
{
    int mid;

    if (low == high)
        return low;

    mid = (low + ((high - low) / 2));

    if (value > array[mid])
    {
        return recursiveBinary(value, mid + 1, high, array);
    }
    else if (value < array[mid])
    {
        return recursiveBinary(value, low, mid, array);
    }

    return mid;
}

/* ----------------------------------------------------------- 
* FUNCTION  merge : (function name)                          
*     Concurrently merge two arrays into a shared output                     
* PARAMETER USAGE :                                           
*    leftSize, left, rightSize, right, output, offset used for printing purposes
* FUNCTION CALLED :                                           
*    memset, ftok, sprintf, shmget, print, shmat, recursiveBinary, shmctrl
* ----------------------------------------------------------- */
void merge(int leftSize, int *left, int rightSize, int *right, int *output, int offSet)
{
    key_t key;
    int shmId, i;
    pid_t pid;
    int *sharedOutput;
    char buff[BUFFERSIZE];
    key = ftok("./", left[0]);
    memset(buff, 0, BUFFERSIZE);
    sprintf(buff, "*** MERGE: shared memory key = %d\n", key);
    print(buff);
    shmId = shmget(key, sizeof(int) * (leftSize + rightSize), IPC_CREAT | 0666);
    sprintf(buff, "*** MERGE: shared memory created\n");
    print(buff);
    sharedOutput = shmat(shmId, NULL, 0);
    sprintf(buff, "*** MERGE: shared memory attached and is ready to use for merging a[%d..%d] and a[%d..%d] into a shared array.\n", offSet, offSet + leftSize - 1, offSet + leftSize, offSet + leftSize + rightSize - 1);
    print(buff);

    /*For all elements, fork and find its place in sharedOutput, then store in shared output*/
    for (i = 0; i < leftSize; i++)
    {
        pid = fork();
        if (pid == 0)
        {

            sprintf(buff, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n", getpid(), getppid(), i + offSet, left[i]);
            print(buff);

            if (left[i] < right[0])
            {
                sharedOutput[i] = left[i];
                sprintf(buff, "      $$$ B-PROC(%d): a[%d] = %d and is smaller than a[%d] = %d and is written to temp[%d]\n", getpid(), i + offSet, left[i], leftSize + offSet, right[0], i);
                print(buff);
            }
            else if (left[i] > right[rightSize - 1])
            {
                sharedOutput[rightSize + i] = left[i];
                sprintf(buff, "      $$$ B-PROC(%d): a[%d] = %d and is larger than a[%d] = %d and is written to temp[%d]\n", getpid(), i + offSet, left[i], leftSize + rightSize - 1 + offSet, right[rightSize - 1], rightSize + i);
                print(buff);
            }
            else
            {
                int temp;
                temp = recursiveBinary(left[i], 0, rightSize - 1, right);
                sharedOutput[temp + i] = left[i];
                sprintf(buff, "      $$$ B-PROC(%d): a[%d] = %d and is between a[%d] = %d and a[%d] = %d is written to temp[%d]\n", getppid(), i + offSet, left[i], leftSize + offSet, right[0], leftSize + rightSize - 1 + offSet, right[rightSize - 1], temp + i);
                print(buff);
            }
            shmdt(sharedOutput);
            sprintf(buff, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is terminated\n", getpid(), getppid(), i + offSet, left[i]);
            print(buff);
            exit(0);
        }
    }

    for (i = 0; i < rightSize; i++)
    {
        pid = fork();
        if (pid == 0)
        {

            sprintf(buff, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n", getpid(), getppid(), i + offSet + leftSize, right[i]);
            print(buff);

            if (right[i] < left[0])
            {
                sharedOutput[i] = right[i];
                sprintf(buff, "      $$$ B-PROC(%d): a[%d] = %d and is smaller than a[%d] = %d and is written to temp[%d]\n", getpid(), i + offSet + leftSize, right[i], offSet, left[0], i);
                print(buff);
            }
            else if (right[i] > left[leftSize - 1])
            {
                sharedOutput[leftSize + i] = right[i];
                sprintf(buff, "      $$$ B-PROC(%d): a[%d] = %d and is larger than a[%d] = %d and is written to temp[%d]\n", getpid(), i + offSet + leftSize, right[i], leftSize - 1 + offSet, left[rightSize - 1], leftSize + i);
                print(buff);
            }
            else
            {
                int temp;
                temp = recursiveBinary(right[i], 0, leftSize - 1, left);
                sharedOutput[temp + i] = right[i];
                sprintf(buff, "      $$$ B-PROC(%d): a[%d] = %d and is between a[%d] = %d and a[%d] = %d is written to temp[%d]\n", getpid(), i + offSet + leftSize, right[i], offSet, left[0], leftSize - 1 + offSet, left[leftSize - 1], temp + i);
                print(buff);
            }
            shmdt(sharedOutput);
            sprintf(buff, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is terminated\n", getpid(), getppid(), i + offSet, left[i]);
            print(buff);
            exit(0);
        }
    }

    while (wait(NULL) != -1);

    /*Copy local shared memory to external shared*/
    for (i = 0; i < (leftSize + rightSize); i++)
    {
        output[i] = sharedOutput[i];
    }

    shmdt(sharedOutput);
    sprintf(buff, "*** MERGE: merging a[%d..%d] and a[%d..%d] into a shared array shared memory sucessfully detached.\n", offSet, offSet + leftSize - 1, offSet + leftSize, offSet + leftSize + rightSize - 1);
    print(buff);
    shmctl(shmId, IPC_RMID, NULL);
    sprintf(buff, "*** MERGE: merging a[%d..%d] and a[%d..%d] into a shared array shared memory sucessfully removed.\n", offSet, offSet + leftSize - 1, offSet + leftSize, offSet + leftSize + rightSize - 1);
    print(buff);
}

/* ----------------------------------------------------------- 
* FUNCTION  main : (function name)                          
*     Recursive function to implement a concurrent merge sort                  
* PARAMETER USAGE :                                           
*    argc, agrv
* FUNCTION CALLED :                                           
*    memset, ftok, sprintf, shmget, print, shmat, recursiveBinary, shmctrl, fork, perror, execvp
* ----------------------------------------------------------- */
int main(int argc, char *argv[])
{
    int i;
    int *sharedElements;
    int *output;
    int shmId;
    int leftSize, rightSize, leftIndex, rightIndex;
    int *left, *right;
    int offSet;
    int topParent;
    char buff[BUFFERSIZE];

    char *arguments[7];
    pid_t pid;

    memset(buff, 0, BUFFERSIZE);
    memset(arguments, 0, sizeof(char *) * 7);

    shmId = atoi(argv[1]);
    leftSize = atoi(argv[2]);
    leftIndex = atoi(argv[3]);
    rightSize = atoi(argv[4]);
    rightIndex = atoi(argv[5]);
    offSet = atoi(argv[6]);

    /*Keep track if you are the top most parent, for printing reasons*/
    topParent = 0;

    left = (int *)malloc(sizeof(int) * leftSize);
    right = (int *)malloc(sizeof(int) * rightSize);
    sharedElements = shmat(shmId, NULL, 0);

    /*Offset = -1 only if this is the first merge call*/
    if (offSet == -1)
    {
        sprintf(buff, "*** MERGE: shared memory attached and is ready to use for getting a[%d..%d] and returning sorted elements.\n", 0, leftSize + rightSize - 1);
        print(buff);
    }
    else
    {
        sprintf(buff, "*** MERGE: shared memory attached and is ready to use for getting a[%d..%d].\n", leftIndex, rightSize + rightIndex - 1);
        print(buff);
    }

    /*Seperate left and right arrays from shared memory*/
    for (i = 0; i < leftSize; i++)
    {
        left[i] = sharedElements[leftIndex + i];
    }
    for (i = 0; i < rightSize; i++)
    {
        right[i] = sharedElements[rightIndex + i];
    }


    /*Print all elements for the merge*/
    if (offSet == -1)
    {
        /*First merge process*/
        sprintf(buff, "   ### M-PROC(%d): entering with a[%d..%d]\n   ", getpid(), leftIndex, rightIndex + rightSize - 1);
        for (i = 0; i < leftSize; i++)
        {
            sprintf(buff + strlen(buff), "   %d", left[i]);
        }
        for (i = 0; i < rightSize; i++)
        {
            sprintf(buff + strlen(buff), "   %d", right[i]);
        }
        sprintf(buff + strlen(buff), " <------- elements of a[%d] to a[%d]\n", leftIndex, (rightIndex + rightSize) - 1);
        print(buff);
        offSet = 0;
        topParent = 1;
    }
    else
    {
        sprintf(buff, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d]\n   ", getpid(), getppid(), leftIndex, rightIndex + rightSize - 1);
        for (i = 0; i < leftSize; i++)
        {
            sprintf(buff + strlen(buff), "   %d", left[i]);
        }
        for (i = 0; i < rightSize; i++)
        {
            sprintf(buff + strlen(buff), "   %d", right[i]);
        }
        sprintf(buff + strlen(buff), " <------- elements of a[%d] to a[%d]\n", leftIndex, (rightIndex + rightSize) - 1);
        print(buff);
    }

    /*Only two elements are left, no need to split, sort them*/
    if (leftSize == 1 && rightSize == 1)
    {
        if (sharedElements[leftIndex] > sharedElements[rightIndex])
        {
            int temp = sharedElements[leftIndex];
            sharedElements[leftIndex] = sharedElements[rightIndex];
            sharedElements[rightIndex] = temp;
        }
        sprintf(buff, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d] -- sorted\n   ", getpid(), getppid(), leftIndex, rightIndex + rightSize - 1);

        sprintf(buff + strlen(buff), "   %d", sharedElements[leftIndex]);

        sprintf(buff + strlen(buff), "   %d", sharedElements[rightIndex]);

        sprintf(buff + strlen(buff), " <------- sorted elements of a[%d] to a[%d]\n", leftIndex, (rightIndex + rightSize) - 1);
        print(buff);
        shmdt(sharedElements);
        exit(0);
    }

    /*More than two elements are left, split into left and right and call merge*/
    pid = fork();
    if (pid == 0)
    {
        /*Left child*/

        arguments[0] = _strdup(argv[0]);
        /*shmptr*/
        arguments[1] = _strdup(argv[1]);
        /*left size*/
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", leftSize / 2);
        arguments[2] = _strdup(buff);
        /*left index*/
        arguments[3] = argv[3];
        /*right size*/
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", leftSize / 2);
        arguments[4] = _strdup(buff);
        /*right index*/
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", rightIndex - (leftSize / 2));
        arguments[5] = _strdup(buff);
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", offSet);
        arguments[6] = _strdup(buff);
        ;
        arguments[7] = NULL;
        execvp("./merge", arguments);
        perror("Error");
    }

    pid = fork();
    if (pid == 0)
    {
        /*Right child*/

        arguments[0] = _strdup(argv[0]);
        /*shmptr*/
        arguments[1] = _strdup(argv[1]);
        /*left size*/
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", rightSize / 2);
        arguments[2] = _strdup(buff);
        /*left index*/
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", rightIndex);
        arguments[3] = _strdup(buff);
        /*right size*/
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", rightSize / 2);
        arguments[4] = _strdup(buff);
        /*right index*/
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", rightIndex + (rightSize / 2));
        arguments[5] = _strdup(buff);
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%d", offSet + (rightSize / 2));
        arguments[6] = _strdup(buff);
        arguments[7] = NULL;
        execvp("./merge", arguments);
        perror("Error");
    }

    /*Wait until children die and the arrays are sorted*/
    while (wait(NULL) != -1);

    /*Should not happen, but for sanity check*/
    if (leftSize <= 1 || rightSize <= 1)
    {
        exit(0);
    }

    sprintf(buff, "   ### M-PROC(%d) created by M-PROC(%d): both array sections sorted. start merging\n", getpid(), getppid());
    print(buff);

    /*Split shared memory into left and righ arrays again, this is needed because the shared memory is holding the sorted values*/
    for (i = 0; i < rightSize; i++)
    {
        right[i] = sharedElements[rightIndex + i];
    }
    for (i = 0; i < leftSize; i++)
    {
        left[i] = sharedElements[leftIndex + i];
    }

    output = (int *)malloc(sizeof(int) * (leftSize + rightSize));

    memset(output, 0, sizeof(int) * (leftSize + rightSize));


    /*Merge the two arrays into one sorted array*/
    merge(leftSize, left, rightSize, right, output, offSet);

    /*topParent means the first call of merge, print all sorted elements*/
    if (topParent == 1)
    {
        sprintf(buff, "   ### M-PROC(%d): entering with a[%d..%d] completed\n   ", getpid(), leftIndex, leftSize + rightSize - 1);
        for (i = 0; i < (leftSize + rightSize); i++)
        {
            sprintf(buff + strlen(buff), "   %d", output[i]);
            sharedElements[leftIndex + i] = output[i];
        }
        sprintf(buff + strlen(buff), "\n");
        print(buff);
    }
    else
    {
        sprintf(buff, "   ### M-PROC(%d) created by M-PROC(%d): merge sort a[%d..%d] completed:\n   ", getpid(), getppid(), leftIndex, rightIndex + rightSize - 1);
        for (i = 0; i < (leftSize + rightSize); i++)
        {
            sprintf(buff + strlen(buff), "   %d", output[i]);
            sharedElements[leftIndex + i] = output[i];
        }
        sprintf(buff + strlen(buff), "\n");
        print(buff);
    }

    shmdt(sharedElements);
    sprintf(buff, "*** MERGE: getting elements and returning sorted elements shared memory sucessfully detached.\n");
    print(buff);
    free(left);
    free(right);
    exit(0);
}
