// -----------------------------------------------------------
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread-support.h
// PROGRAM PURPOSE :
//   Delarations of functions in thread-support.cpp
// -----------------------------------------------------------

#ifndef THREAD_SUPPORT_H
#define THREAD_SUPPORT_H

#include "ThreadClass.h"
#include <stdio.h>
#include <string.h>

#define ELF_NUMBER_QUESTIONS 3

extern Semaphore *SantaSleeping;
extern Semaphore *Reindeer;
extern Semaphore *Elf;
extern Semaphore *Sleigh;
extern Semaphore *ElfHasQuestion;
extern Semaphore *ElfsQuestionAnswered;
extern Semaphore *ReindeerIsDoneFlying;
extern Semaphore *WaitForOtherReindeer;
extern Semaphore *ReindeerIsAttached;
extern Semaphore *ReindeerIsUnAttached;

extern Mutex *ElfCounterLock;
extern Mutex *ReindeerCounterLock;

extern int elfCounter;
extern int reindeerCounter;
extern int *waitingElfs;

// -----------------------------------------------------------
// FUNCTION  _print
//     Wrapper for writing to a buff to console
// PARAMETER USAGE :
//    buff
// FUNCTION CALLED :
//	  write, strlen
// -----------------------------------------------------------
void _print(char *buff);

// -----------------------------------------------------------
// FUNCTION  AskQuestion
//     Called when each elf has a question, will wait for
//	   santa to answer
// PARAMETER USAGE :
//    i index of the elf
// FUNCTION CALLED :
//    sprintf, _print, wait, lock, signal, unlock
// -----------------------------------------------------------
void AskQuestion(int i);

// -----------------------------------------------------------
// FUNCTION  ReindeerBack
//     Reindeer checks in that he is back, if they are the
//	   last, they wake up santa
// PARAMETER USAGE :
//    indexOfReindeer, numberOfReindeer
// FUNCTION CALLED :
//    sprintf, _print, signal, unlock, lock
// -----------------------------------------------------------
void ReindeerBack(int indexOfReindeer, int numberOfReindeer);

// -----------------------------------------------------------
// FUNCTION  WaitOthers
//     Waits for all reindeer to return
// FUNCTION CALLED :
//    wait
// -----------------------------------------------------------
void WaitOthers();

// -----------------------------------------------------------
// FUNCTION  FlyOff
//     Wait to be attached to the sleigh, then fly, once back
//     unattach yourself from the sleigh
// FUNCTION CALLED :
//    signal, weight
// -----------------------------------------------------------
void FlyOff();

// -----------------------------------------------------------
// FUNCTION  Sleep
//     Santa is sleeping
// FUNCTION CALLED :
//    wait
// -----------------------------------------------------------
void Sleep();

// -----------------------------------------------------------
// FUNCTION  WaitSleigh
//     Wait on the sleigh to be ready
// FUNCTION CALLED :
//    wait
// -----------------------------------------------------------
void WaitSleigh();
#endif