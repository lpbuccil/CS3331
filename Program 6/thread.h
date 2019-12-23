// -----------------------------------------------------------
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 12/13/2019
// PROGRAM ASSIGNMENT 6
// FILE NAME : thread.h (your unix file name)
// PROGRAM PURPOSE :
//    Definitions for thead.cpp
// -----------------------------------------------------------

#ifndef THREAD_H
#define THREAD_H

#include "ThreadClass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define EOD -1

extern int A_row;
extern int A_col;
extern int B_row;
extern int B_col;
extern int** aElements;
extern int** bElements;
extern int** output;

class ThreadProcessor : public Thread {
public:
    ThreadProcessor(char*, int, int, int);
    // -----------------------------------------------------------
    // FUNCTION  getRightChannel : (function name)
    //     returns the righ channel of the thread
    // -----------------------------------------------------------
    SynOneToOneChannel* getRightChannel();
    // -----------------------------------------------------------
    // FUNCTION  getBelowChannel : (function name)
    //     returns the below channel of the thread
    // -----------------------------------------------------------
    SynOneToOneChannel* getBelowChannel();

    int threadId;
    int processerAbove;
    int processorBelow;
    int processorLeft;
    int processorRight;
    // -----------------------------------------------------------
    // FUNCTION  setLeftChannel : (function name)
    //     sets the left channel of the thread
    // PARAMETER USAGE :
    //    SynOneToOneChannel
    // -----------------------------------------------------------
    void setLeftChannel(SynOneToOneChannel*);
    // -----------------------------------------------------------
    // FUNCTION  setRightChannel : (function name)
    //     sets the right channel of the thread
    // PARAMETER USAGE :
    //    SynOneToOneChannel
    // -----------------------------------------------------------
    void setRightChannel(SynOneToOneChannel*);
    // -----------------------------------------------------------
    // FUNCTION  setBelowChannel : (function name)
    //     sets the below channel of the thread
    // PARAMETER USAGE :
    //    SynOneToOneChannel
    // -----------------------------------------------------------
    void setBelowChannel(SynOneToOneChannel*);
    // -----------------------------------------------------------
    // FUNCTION  setAboveChannel : (function name)
    //     sets the above channel of the thread
    // PARAMETER USAGE :
    //    SynOneToOneChannel
    // -----------------------------------------------------------
    void setAboveChannel(SynOneToOneChannel*);

private:
    // -----------------------------------------------------------
    // FUNCTION  ThreadFunc : (function name)
    //     Gets data from left and above channel, then forwards
    //	   the data respectivly.
    // FUNCTION CALLED :
    //    sprintf, _print, Receive, Send, Exit
    // -----------------------------------------------------------
    void ThreadFunc();

    int indexI;
    int indexJ;

    SynOneToOneChannel* aboveChannel;
    SynOneToOneChannel* belowChannel;
    SynOneToOneChannel* rightChannel;
    SynOneToOneChannel* leftChannel;
};

class ThreadColumnChannel : public Thread {
public:
    ThreadColumnChannel(char*, int, int);

    int threadId;
    // -----------------------------------------------------------
    // FUNCTION  getChannel : (function name)
    //     sets the thread channel
    // -----------------------------------------------------------
    SynOneToOneChannel* getChannel();
    // -----------------------------------------------------------
    // FUNCTION  setChannel : (function name)
    //     sets the channel of the thread
    // PARAMETER USAGE :
    //    SynOneToOneChannel
    // -----------------------------------------------------------
    void setChannel(SynOneToOneChannel* channel);

private:
    // -----------------------------------------------------------
    // FUNCTION  ThreadFunc : (function name)
    //     Gets data from left and above channel, then forwards
    //	   the data respectivly.
    // FUNCTION CALLED :
    //    sprintf, _print, Receive, Send, Exit
    // -----------------------------------------------------------
    void ThreadFunc();

    int index;
    SynOneToOneChannel* channel;
};

class ThreadRowChannel : public Thread {
public:
    ThreadRowChannel(char*, int, int);

    int threadId;

    // -----------------------------------------------------------
    // FUNCTION  getChannel : (function name)
    //     returns the thread channel
    // -----------------------------------------------------------
    SynOneToOneChannel* getChannel();
    // -----------------------------------------------------------
    // FUNCTION  setChannel : (function name)
    //     sets the below channel of the thread
    // PARAMETER USAGE :
    //    SynOneToOneChannel
    // -----------------------------------------------------------
    void setChannel(SynOneToOneChannel* channel);

private:
    // -----------------------------------------------------------
    // FUNCTION  ThreadFunc : (function name)
    //     Gets data from left and above channel, then forwards
    //	   the data respectivly.
    // FUNCTION CALLED :
    //    sprintf, _print, Receive, Send, Exit
    // -----------------------------------------------------------
    void ThreadFunc();

    int index;
    SynOneToOneChannel* channel;
};
#endif