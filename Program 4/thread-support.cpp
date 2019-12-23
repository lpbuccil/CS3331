// -----------------------------------------------------------
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread-support.cpp
// PROGRAM PURPOSE :
//    Contains all of the logic for each
//    thread, function comments are in thread-support.h
// -----------------------------------------------------------

#include "thread-support.h"

void _print(char *buff)
{
    write(1, buff, strlen(buff));
}

void WaitOthers()
{
    WaitForOtherReindeer->Wait();
}

void WaitSleigh()
{
    Sleigh->Wait();
}

void FlyOff()
{
    ReindeerIsAttached->Signal();
    ReindeerIsDoneFlying->Wait();
    ReindeerIsUnAttached->Signal();
}

void Sleep()
{
    SantaSleeping->Wait();
}

void AskQuestion(int indexOfElf)
{
    char buff[100];
    int i;
    sprintf(buff, "         Elf %d has a problem\n", indexOfElf + 1);
    _print(buff);

    //check if there are elves asking questions already
    Elf->Wait();

    ElfCounterLock->Lock();
    waitingElfs[elfCounter] = indexOfElf + 1;
    elfCounter++;

    if (elfCounter == 3)
    {
        sprintf(buff, "         Elves %d, %d, %d wake up the Santa\n", waitingElfs[0], waitingElfs[1], waitingElfs[2]);
        _print(buff);
        SantaSleeping->Signal();
    }
    ElfCounterLock->Unlock();

    //ask question and wait for santa to answer
    ElfHasQuestion->Wait();

    //wait for all questions to be answered
    ElfsQuestionAnswered->Wait();

    ElfCounterLock->Lock();
    elfCounter--;

    //all questions have been answered and can return to work
    if (elfCounter == 0)
    {
        sprintf(buff, "         Elves %d, %d, %d return to work\n", waitingElfs[0], waitingElfs[1], waitingElfs[2]);
        _print(buff);
        waitingElfs[0] = 0;
        waitingElfs[1] = 0;
        waitingElfs[2] = 0;
        //let the next elves with questions ask
        for (i = 0; i < ELF_NUMBER_QUESTIONS; i++)
        {
            Elf->Signal();
        }
    }
    ElfCounterLock->Unlock();
};

void ReindeerBack(int indexOfReindeer, int numberOfReindeer)
{
    char buff[100];
    int i;
    sprintf(buff, "     Reindeer %d returns\n", indexOfReindeer + 1);
    _print(buff);
    ReindeerCounterLock->Lock();
    reindeerCounter++;
    //if last reindeer, tell all others that everyone is back and wake up santa
    if (reindeerCounter == numberOfReindeer)
    {
        for (i = 0; i < numberOfReindeer; i++)
        {
            WaitForOtherReindeer->Signal();
        }
        sprintf(buff, "     The last reindeer %d wakes up Santa\n", indexOfReindeer + 1);
        _print(buff);
        SantaSleeping->Signal();
    }
    ReindeerCounterLock->Unlock();
};