// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread.cpp (your unix file name)            
// PROGRAM PURPOSE :                                           
//    Implementation of thead.cpp, provides thread logic.       
// ----------------------------------------------------------- 

#ifndef THREAD_CPP
#define THREAD_CPP
#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  _print : (function name)                          
//     tprint wrapper                          
// PARAMETER USAGE :                                           
//    buff               
// FUNCTION CALLED :                                           
//    write, memset      
// ----------------------------------------------------------- 
void _print(char buff[100])
{
    write(1, buff, strlen(buff));
    memset(buff, '\0', sizeof(buff));
}

ThreadColumnChannel::ThreadColumnChannel(char *name, int index, int threadId)
    : Thread(name), index(index), threadId(threadId)
{
    UserDefinedThreadId = threadId;
    channel = NULL;
};

ThreadRowChannel::ThreadRowChannel(char *name, int index, int threadId)
    : Thread(name), index(index), threadId(threadId)
{
    UserDefinedThreadId = threadId;
    channel = NULL;
};

ThreadProcessor::ThreadProcessor(char *name, int i, int j, int threadId)
    : Thread(name), threadId(threadId), indexI(i), indexJ(j)
{
    UserDefinedThreadId = threadId;
    leftChannel = NULL;
    rightChannel = NULL;
    aboveChannel = NULL;
    belowChannel = NULL;
    processerAbove = FALSE;
    processorBelow = FALSE;
    processorRight = FALSE;
    processorLeft = FALSE;
};

void ThreadColumnChannel::ThreadFunc()
{
    char buff[100];
    int i;
    int element;
    sprintf(buff, "   Column thread c[%d] started\n", index);
    _print(buff);
    for (i = 0; i < B_row; i++)
    {
        element = bElements[i][index];
        channel->Send(&element, sizeof(int));
        sprintf(buff, "   Column thread c[%d] sent %d to P[%d,%d]\n", index, element, 0, index);
        _print(buff);
    }
    element = EOD;
    channel->Send(&element, sizeof(int));
    sprintf(buff, "   Column thread c[%d] sent EOD to P[%d,%d] and terminated\n", index, 0, index);
    _print(buff);
    Exit();
}

void ThreadRowChannel::ThreadFunc()
{
    char buff[100];
    int i;
    int element;
    sprintf(buff, "Row thread r[%d] started\n", index);
    _print(buff);
    for (i = 0; i < A_col; i++)
    {
        element = aElements[index][i];
        channel->Send(&element, sizeof(int));
        sprintf(buff, "Row thread r[%d] sent %d to P[%d,%d]\n", index, element, index, 0);
        _print(buff);
    }
    element = EOD;
    channel->Send(&element, sizeof(int));
    sprintf(buff, "Row thread r[%d] sent EOD to P[%d,%d] and terminated\n", index, index, 0);
    _print(buff);
    Exit();
}

void ThreadProcessor::ThreadFunc()
{
    char buff[100];
    int i;
    int a, b;
    int tempOutput;
    bool aDone, bDone;
    sprintf(buff, "      Thread P[%d,%d] started\n", indexI, indexJ);
    _print(buff);
    tempOutput = 0;
    aDone = false;
    bDone = false;
    while (1)
    {
        //check if we need to receive from above
        if (aDone == false)
        {
            aboveChannel->Receive(&a, sizeof(int));
        }
        //chack if we need to receive from left
        if (bDone == false)
        {
            leftChannel->Receive(&b, sizeof(int));
        }

        //print statements
        if (aDone == false && bDone == false)
        {
            sprintf(buff, "      Thread P[%d,%d] received %d from above and %d from left\n", indexI, indexJ, a, b);
            _print(buff);
        }
        else if (aDone == true && bDone == false)
        {
            sprintf(buff, "      Thread P[%d,%d] received %d from left\n", indexI, indexJ, b);
            _print(buff);
        }
        else if (aDone == false && bDone == true)
        {
            sprintf(buff, "      Thread P[%d,%d] received %d from above\n", indexI, indexJ, a);
            _print(buff);
        }
        else if (aDone == true && bDone == true)
        {
            //we have recevied all input
            sprintf(buff, "      Thread P[%d,%d] received EOD, saved result %d and terminated\n", indexI, indexJ, tempOutput);
            _print(buff);
            output[indexI][indexJ] = tempOutput;
            Exit();
        }

        if (processorRight == TRUE && processorBelow == TRUE)
        {
            sprintf(buff, "      Thread P[%d,%d] sent %d to below and %d to right\n", indexI, indexJ, a, b);
            _print(buff);
        }
        else if (processorRight == FALSE && processorBelow == TRUE)
        {
            sprintf(buff, "      Thread P[%d,%d] sent %d to below\n", indexI, indexJ, a);
            _print(buff);
        }
        else if (processorRight == TRUE && processorBelow == FALSE)
        {
            sprintf(buff, "      Thread P[%d,%d] sent %d to right\n", indexI, indexJ, b);
            _print(buff);
        }

        //we have received an EOD
        if (a == EOD || b == EOD)
        {
            //a sent it
            if (a = EOD)
            {
                //dont try to receive from above
                aDone = true;
                //if we have a processor below, send it EOD
                if (processorBelow == TRUE)
                {
                    belowChannel->Send(&a, sizeof(int));
                }
            }

            if (b == EOD)
            {
                bDone = true;
                if (processorRight == TRUE)
                {
                    rightChannel->Send(&b, sizeof(int));
                }
            }
        }
        //was not an EOD
        else
        {
            if (processorRight == TRUE)
            {
                rightChannel->Send(&b, sizeof(int));
            }
            if (processorBelow == TRUE)
            {
                belowChannel->Send(&a, sizeof(int));
            }
            tempOutput += (a * b);
        }
    }
}

void ThreadProcessor::setAboveChannel(SynOneToOneChannel *channel)
{
    aboveChannel = channel;
}

void ThreadProcessor::setRightChannel(SynOneToOneChannel *channel)
{
    rightChannel = channel;
}

void ThreadProcessor::setLeftChannel(SynOneToOneChannel *channel)
{
    leftChannel = channel;
}

void ThreadProcessor::setBelowChannel(SynOneToOneChannel *channel)
{
    belowChannel = channel;
}

SynOneToOneChannel *ThreadProcessor::getRightChannel()
{
    return rightChannel;
}

SynOneToOneChannel *ThreadProcessor::getBelowChannel()
{
    return belowChannel;
}

SynOneToOneChannel *ThreadRowChannel::getChannel()
{
    return channel;
}

void ThreadRowChannel::setChannel(SynOneToOneChannel *cha)
{
    channel = cha;
}

SynOneToOneChannel *ThreadColumnChannel::getChannel()
{
    return channel;
}

void ThreadColumnChannel::setChannel(SynOneToOneChannel *cha)
{
    channel = cha;
}

#endif