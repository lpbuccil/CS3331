// -----------------------------------------------------------
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//    Main program to create semaphores, mutexs, counters, and
//    waitingElfs array. Creates the santa, reindeer, and elves threads
// -----------------------------------------------------------
#include "ThreadClass.h"
#include "thread.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int elfCounter;
int reindeerCounter;

Semaphore *SantaSleeping;
Semaphore *Sleigh;
Semaphore *ElfHasQuestion;
Semaphore *Elf;
Semaphore *ReindeerIsDoneFlying;
Semaphore *ReindeerIsAttached;
Semaphore *ReindeerIsUnAttached;
Semaphore *ElfsQuestionAnswered;
Semaphore *WaitForOtherReindeer;

Mutex *ElfCounterLock;
Mutex *ReindeerCounterLock;

int *waitingElfs;

// -----------------------------------------------------------
// FUNCTION  main
//     entry point of program
// PARAMETER USAGE :
//    argv <elfs> <reindeer> <trips>
// FUNCTION CALLED :
//    begin, join, exit, memset, sprintf
// -----------------------------------------------------------
int main(int argc, char **argv)
{
    int numberOfReindeer, numberOfElfs, numberOfTrips;
    int i;
    char buff[100];

    reindeerCounter = 0;
    elfCounter = 0;
    waitingElfs = new int[3];

    if (argc != 4)
    {
        cout << "Usage ./prog4 <number of elfs> <number of reindeer> <number of trips>" << endl;
        exit(1);
    }

    numberOfReindeer = atoi(argv[2]);
    if (numberOfReindeer == 0)
    {
        numberOfReindeer = 9;
    }

    numberOfElfs = atoi(argv[1]);
    if (numberOfElfs == 0)
    {
        numberOfElfs = 7;
    }

    numberOfTrips = atoi(argv[3]);
    if (numberOfTrips == 0)
    {
        numberOfTrips = 5;
    }

    Elf = new Semaphore("Elf", ELF_NUMBER_QUESTIONS);
    Sleigh = new Semaphore("Sleigh", 0);
    ReindeerIsAttached = new Semaphore("ReindeerIsAttached", 0);
    ReindeerIsDoneFlying = new Semaphore("ReindeerIsDoneFlying", 0);
    ReindeerIsUnAttached = new Semaphore("ReindeerIsUnAttached", 0);
    ElfHasQuestion = new Semaphore("ElfHasQuestion", 0);
    ElfsQuestionAnswered = new Semaphore("ElfsQuestionAnswered", 0);
    SantaSleeping = new Semaphore("SantaSleeping", 0);
    WaitForOtherReindeer = new Semaphore("WaitForOtherReindeer", 0);

    ReindeerCounterLock = new Mutex("ReindeerCounterLock");
    ElfCounterLock = new Mutex("ElfCounterLock");

    ReindeerThread *reindeer[numberOfReindeer];
    ElfThread *elf[numberOfElfs];

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "Santa");
    SantaThread *santa = new SantaThread(buff, numberOfTrips, numberOfReindeer);

    for (i = 0; i < numberOfElfs; i++)
    {
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "Elf(%d)", i + 1);
        elf[i] = new ElfThread(buff, i);
    }

    for (i = 0; i < numberOfReindeer; i++)
    {
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "Reindeer(%d)", i + 1);
        reindeer[i] = new ReindeerThread(buff, i, numberOfReindeer);
    }

    santa->Begin();

    if (numberOfElfs >= numberOfReindeer)
    {
        for (i = 0; i < numberOfElfs; i++)
        {
            elf[i]->Begin();
            if (i < numberOfReindeer)
            {
                reindeer[i]->Begin();
            }
        }
    }
    else
    {
        for (i = 0; i < numberOfReindeer; i++)
        {
            reindeer[i]->Begin();
            if (i < numberOfElfs)
            {
                elf[i]->Begin();
            }
        }
    }

    //wait for santa to retire
    santa->Join();
    exit(0);
}