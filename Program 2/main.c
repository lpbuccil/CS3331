/* ----------------------------------------------------------- 
* NAME : Lucas Buccilli                         User ID: lpbuccil 
* DUE DATE : 10/18/2019                                       
* PROGRAM ASSIGNMENT 2                                        
* FILE NAME : main.c (your unix file name)            
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
* FUNCTION  main : (function name)                          
*     Main method to drive the concurrent merge sort                         
* PARAMETER USAGE :                                                     
* FUNCTION CALLED :                                           
*    memset, shmget, ftok, printf, shmctl, exit, shmat, scanf, sprintf, shmdt        
* ----------------------------------------------------------- */
int main()
{
    int numberOfElements;
    int i;
    int *sharedElements;
    int shmId;
    key_t key;
    char *arguments[100];
    char buff[BUFFERSIZE];
    memset(buff, 0, sizeof(buff));
    memset(arguments, 0, sizeof(char *) * 100);

    scanf("%d", &numberOfElements);
    key = ftok("./", 'a');
    printf("*** MAIN: shared memory key = %d\n", key);

    shmId = shmget(key, sizeof(int) * numberOfElements, IPC_CREAT | 0666);
    if (shmId >= 0)
    {
        printf("*** MAIN: shared memory created\n");
    }
    else
    {
        printf("*** MAIN: shared memory FAILED to create\n");
        shmctl(shmId, IPC_RMID, NULL);
        exit(1);
    }

    sharedElements = shmat(shmId, NULL, 0);
    if (*sharedElements >= 0)
    {
        printf("*** MAIN: shared memory attached and is ready to use\n\n");
    }
    else
    {
        printf("*** MAIN: shared memory FAILED to attach shared memory\n");
        exit(1);
    }

    printf("Input array for mergesort has %d elements:\n", numberOfElements);

    for (i = 0; i < numberOfElements; i++)
    {
        scanf("%d", &sharedElements[i]);
        printf("   %d", sharedElements[i]);
    }
    printf("\n\n");

    /*Call merge with the array*/
    arguments[0] = "merge";
    /*1 SHMID*/
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%d", shmId);
    arguments[1] = _strdup(buff);
    /*2 Left size*/
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%d", numberOfElements / 2);
    arguments[2] = _strdup(buff);
    /*3 Left index*/
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%d", 0);
    arguments[3] = _strdup(buff);
    /*4 Right size*/
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%d", numberOfElements / 2);
    arguments[4] = _strdup(buff);
    /*5 Right Index*/
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%d", numberOfElements / 2);
    arguments[5] = _strdup(buff);
    arguments[6] = "-1";
    arguments[7] = NULL;
    printf("*** MAIN: about to spawn the merge sort process\n");
    if (fork() == 0)
    {
        execvp("./merge", arguments);
    }

    /*Wait until child dies and array is sorted*/
    wait(NULL);

    printf("*** MAIN: merged array:\n   ");
    for (i = 0; i < numberOfElements; i++)
    {
        printf("   %d", sharedElements[i]);
    }
    printf("\n");
    shmdt(sharedElements);
    printf("*** MAIN: shared memory successfully detached\n");
    shmctl(shmId, IPC_RMID, NULL);
    printf("*** MAIN: shared memory successfully removed\n");
    printf("*** MAIN: exits\n");

    return 0;
}
