// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread-main.cpp (your unix file name)            
// PROGRAM PURPOSE :                                           
//    Implementation of thead-main.cpp, entry point of program
//    and logic to read the input and start the threads.        
// ----------------------------------------------------------- 

#include "thread.h"

int A_row, A_col;
int B_row, B_col;
int **aElements, **bElements;
int **output;

// ----------------------------------------------------------- 
// FUNCTION  _print1 : (function name)                          
//     tprint wrapper                          
// PARAMETER USAGE :                                           
//    buff               
// FUNCTION CALLED :                                           
//    write, memset      
// ----------------------------------------------------------- 
void _print1(char buff[100])
{
    write(1, buff, strlen(buff));
    memset(buff, '\0', sizeof(buff));
}

int main(int argc, char *argv[])
{
    int P_col, P_row;
    int i, j;
    int id;

    char buff[100];
    ThreadProcessor ***threadProcessor;
    ThreadRowChannel **threadRowChannel;
    ThreadColumnChannel **threadColumnChannel;

    //Read in elements for A
    scanf("%d", &A_row);
    scanf("%d", &A_col);
    aElements = (int **)malloc(A_row * sizeof(int *));

    for (i = 0; i < A_row; i++)
    {
        aElements[i] = (int *)malloc(A_col * sizeof(int));
        for (j = 0; j < A_col; j++)
        {
            scanf("%d", &aElements[i][j]);
        }
    }

    //Read in elements for B
    scanf("%d", &B_row);
    scanf("%d", &B_col);
    bElements = (int **)malloc(B_row * sizeof(int *));

    for (i = 0; i < B_row; i++)
    {
        bElements[i] = (int *)malloc(B_col * sizeof(int));
        for (j = 0; j < B_col; j++)
        {
            scanf("%d", &bElements[i][j]);
        }
    }

    //Check if matricies can be multiplied
    if (A_col != B_row)
    {
        printf("Matrices cannot be multiplied\n");
        for (i = 0; i < A_row; i++)
        {
            free(aElements[i]);
        }

        for (i = 0; i < B_col; i++)
        {
            free(bElements[i]);
        }
        free(aElements);
        free(bElements);
        exit(0);
    }

    P_row = A_row;
    P_col = B_col;

    //create array to hold processor threads
    threadProcessor = (ThreadProcessor ***)malloc(P_row * sizeof(ThreadProcessor **));
    for (i = 0; i < P_row; i++)
    {
        threadProcessor[i] = (ThreadProcessor **)malloc(P_col * sizeof(ThreadProcessor *));
    }

    //create output array
    output = (int **)malloc(P_row * sizeof(int *));
    for (i = 0; i < P_row; i++)
    {
        output[i] = (int *)malloc(P_col * sizeof(int));
    }

    id = 0;

    //create processors of the result matrix size
    for (i = 0; i < P_row; i++)
    {
        for (j = 0; j < P_col; j++)
        {
            sprintf(buff, "processor(%d)[%d][%d]", id, i, j);
            threadProcessor[i][j] = new ThreadProcessor(buff, i, j, id++);

            if (i != 0)
            {
                threadProcessor[i][j]->processerAbove = TRUE;
            }
            if (j != 0)
            {
                threadProcessor[i][j]->processorLeft = TRUE;
            }
            if (j != P_col - 1)
            {
                threadProcessor[i][j]->processorRight = TRUE;
            }
            if (i != P_row - 1)
            {
                threadProcessor[i][j]->processorBelow = TRUE;
            }
        }
    }

    //create processors for the rows
    threadRowChannel = (ThreadRowChannel **)malloc(A_col * sizeof(ThreadRowChannel *));
    for (i = 0; i < P_row; i++)
    {
        sprintf(buff, "RowChannel(%d)[%d]", id, i);
        threadRowChannel[i] = new ThreadRowChannel(buff, i, id++);
    }

    //create processors for the columns
    threadColumnChannel = (ThreadColumnChannel **)malloc(A_col * sizeof(ThreadColumnChannel *));
    for (i = 0; i < P_col; i++)
    {
        sprintf(buff, "ColumnChannel(%d)[%d]", id, i);
        threadColumnChannel[i] = new ThreadColumnChannel(buff, i, id++);
    }

    //channels for sending data to from result matrix processors to result matrix processors
    for (i = 0; i < P_row; i++)
    {
        for (j = 0; j < P_col; j++)
        {
            if (threadProcessor[i][j]->processorRight == TRUE)
            {
                sprintf(buff, "Channel %d to %d", threadProcessor[i][j]->threadId, threadProcessor[i][j + 1]->threadId);
                threadProcessor[i][j]->setRightChannel(new SynOneToOneChannel(buff, threadProcessor[i][j]->threadId, threadProcessor[i][j + 1]->threadId));
                threadProcessor[i][j + 1]->setLeftChannel(threadProcessor[i][j]->getRightChannel());
            }
            if (threadProcessor[i][j]->processorBelow == TRUE)
            {
                sprintf(buff, "Channel %d to %d", threadProcessor[i][j]->threadId, threadProcessor[i + 1][j]->threadId);
                threadProcessor[i][j]->setBelowChannel(new SynOneToOneChannel(buff, threadProcessor[i][j]->threadId, threadProcessor[i + 1][j]->threadId));
                threadProcessor[i + 1][j]->setAboveChannel(threadProcessor[i][j]->getBelowChannel());
            }
        }
    }

    //channels from row processors to the first result matrix processors of each row
    for (i = 0; i < P_row; i++)
    {
        sprintf(buff, "Row Channel %d to %d", threadRowChannel[i]->threadId, threadProcessor[i][0]->threadId);
        threadRowChannel[i]->setChannel(new SynOneToOneChannel(buff, threadRowChannel[i]->threadId, threadProcessor[i][0]->threadId));
        threadProcessor[i][0]->setLeftChannel(threadRowChannel[i]->getChannel());
        threadProcessor[i][0]->processorLeft = TRUE;
    }

    //channels from column processors to the first result matrix processors of each column
    for (i = 0; i < P_col; i++)
    {
        sprintf(buff, "Column Channel %d to %d", threadColumnChannel[i]->threadId, threadProcessor[0][i]->threadId);
        threadColumnChannel[i]->setChannel(new SynOneToOneChannel(buff, threadColumnChannel[i]->threadId, threadProcessor[0][i]->threadId));
        threadProcessor[0][i]->setAboveChannel(threadColumnChannel[i]->getChannel());
        threadProcessor[0][i]->processerAbove = TRUE;
    }

    for (i = 0; i < P_row; i++)
    {
        threadRowChannel[i]->Begin();
    }
    for (i = 0; i < P_col; i++)
    {
        threadColumnChannel[i]->Begin();
    }
    for (i = 0; i < P_row; i++)
    {
        for (j = 0; j < P_col; j++)
        {
            threadProcessor[i][j]->Begin();
        }
    }

    for (i = 0; i < P_row; i++)
    {
        threadRowChannel[i]->Join();
    }
    for (i = 0; i < P_col; i++)
    {
        threadColumnChannel[i]->Join();
    }
    for (i = 0; i < P_row; i++)
    {
        for (j = 0; j < P_col; j++)
        {
            threadProcessor[i][j]->Join();
        }
    }

    printf("\n*** From main ***\n");
    printf("Martix A: %d rows and %d columns\n", A_row, A_col);
    for (i = 0; i < A_row; i++)
    {
        for (j = 0; j < A_col; j++)
        {
            printf("%d ", aElements[i][j]);
        }
        printf("\n");
    }

    printf("\nMartix B: %d rows and %d columns\n", B_row, B_col);
    for (i = 0; i < B_row; i++)
    {
        for (j = 0; j < B_col; j++)
        {
            printf("%d ", bElements[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("Matrix C=A*B: %d rows and %d columns\n", P_row, P_col);
    for (i = 0; i < P_row; i++)
    {
        for (j = 0; j < P_col; j++)
        {
            printf("%d ", output[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    exit(0);
}