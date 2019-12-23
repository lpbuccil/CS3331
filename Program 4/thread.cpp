// -----------------------------------------------------------
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//    Contains all of the thread types and the logic for each
//    thread, function comments are in thread.h
// -----------------------------------------------------------
#include "thread.h"

ElfThread::ElfThread(char *name, int index)
    : index(index), name(name)
{
    ThreadName.seekp(0, ios::beg);
    ThreadName << name;
};

void ElfThread::ThreadFunc()
{
    char buff[100];
    sprintf(buff, "         Elf %d starts\n", index + 1);
    _print(buff);
    while (1)
    {
        Thread::Delay();
        AskQuestion(index);
        Thread::Delay();
    }
};

ReindeerThread::ReindeerThread(char *name, int index, int numberOfReindeer)
    : name(name), index(index), numberOfReindeer(numberOfReindeer)
{
    ThreadName.seekp(0, ios::beg);
    ThreadName << name;
};

void ReindeerThread::ThreadFunc()
{
    char buff[100];
    sprintf(buff, "     Reindeer %d starts\n", index + 1);
    _print(buff);
    while (1)
    {
        Thread::Delay();
        ReindeerBack(index, numberOfReindeer);
        WaitOthers();
        WaitSleigh();
        FlyOff();
        Thread::Delay();
    }
};

SantaThread::SantaThread(char *name, int numberOfTrips, int numberOfReindeer)
    : name(name), currentTrip(0), numberOfTrips(numberOfTrips), numberOfReindeer(numberOfReindeer)
{
    ThreadName.seekp(0, ios::beg);
    ThreadName << name;
};

void SantaThread::ThreadFunc()
{
    char buff[100];
    int i;
    _print("Santa thread starts\n");
    //while we dont retire
    while (currentTrip < numberOfTrips)
    {
        Sleep();
        ReindeerCounterLock->Lock();
        //who woke me up
        if (reindeerCounter == numberOfReindeer)
        {
            _print("Santa is preparing sleighs\n");

            //tell all reindeer the sleigh is ready
            for (i = 0; i < numberOfReindeer; i++)
            {
                Sleigh->Signal();
            }
            //wait for all reindeer to attach to sleigh
            for (i = 0; i < numberOfReindeer; i++)
            {
                ReindeerIsAttached->Wait();
            }

            sprintf(buff, "The team flies off (%d)!\n", currentTrip + 1);
            _print(buff);

            //deliever toys
            Thread::Delay();

            //we are done delivering toys, tell the reindeer
            for (i = 0; i < numberOfReindeer; i++)
            {
                ReindeerIsDoneFlying->Signal();
            }
            //wait for the reindeer to unattach from sleigh
            for (i = 0; i < numberOfReindeer; i++)
            {
                ReindeerIsUnAttached->Wait();
            }

            currentTrip++;
            reindeerCounter = 0;
            ReindeerCounterLock->Unlock();
        }
        else
        {
            ReindeerCounterLock->Unlock();

            _print("Santa is helping elves\n");
            //answer questions
            for (i = 0; i < ELF_NUMBER_QUESTIONS; i++)
            {
                Thread::Delay(); //quesitons may take longer
                ElfHasQuestion->Signal();
            }
            //all questions answered, release all
            for (i = 0; i < ELF_NUMBER_QUESTIONS; i++)
            {
                ElfsQuestionAnswered->Signal();
            }
            sprintf(buff, "Santa answers the question posted by elves %d, %d, %d\n", waitingElfs[0], waitingElfs[1], waitingElfs[2]);
            _print(buff);
        }
    }
    sprintf(buff, "After (%d) deliveries, Santa retires and is on vacation!\n", currentTrip);
    _print(buff);
    Exit();
};
