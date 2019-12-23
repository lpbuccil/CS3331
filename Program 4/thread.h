// -----------------------------------------------------------
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    Delarations of functions in thread.cpp
// -----------------------------------------------------------

#ifndef THREAD_H
#define THREAD_H

#include "ThreadClass.h"
#include "thread-support.h"
#include <stdio.h>
#include <string.h>

class ElfThread : public Thread
{
public:
	// -----------------------------------------------------------
	// FUNCTION  ElfThread
	//     Default constructor
	// PARAMETER USAGE :
	//    <name> of elf, <index> of elf
	// FUNCTION CALLED :
	//    sprintf, _print, signal, unlock, lock
	// -----------------------------------------------------------
	ElfThread(char *name, int index);

private:
	// -----------------------------------------------------------
	// FUNCTION  ElfThread::ThreadFunct
	//     Function of the elf thread
	// FUNCTION CALLED :
	//    Delay, AskQuestion, sprintf, _print
	// -----------------------------------------------------------
	void ThreadFunc();
	char *name;
	int index;
};

class ReindeerThread : public Thread
{
public:
	// -----------------------------------------------------------
	// FUNCTION  ReindeerThread
	//     Default constructor
	// PARAMETER USAGE :
	//    <name> of reindeer, <index> of reindeer, <numberOfReindeer>
	// -----------------------------------------------------------
	ReindeerThread(char *name, int index, int numberOfReindeer);

private:
	// -----------------------------------------------------------
	// FUNCTION  ReindeerThread::ThreadFunct
	//     Function of the reindeer thread
	// FUNCTION CALLED :
	//    Delay, sprintf, _print, ReindeerBack, WaitOthers, WaitSleigh, Flyoff
	// -----------------------------------------------------------
	void ThreadFunc();
	char *name;
	int index, numberOfReindeer;
};
class SantaThread : public Thread
{
public:
	// -----------------------------------------------------------
	// FUNCTION  SantaThread
	//     Default constructor
	// PARAMETER USAGE :
	//    <name> of Santa, <numberOfTrips>, <numberOfReindeer>
	// -----------------------------------------------------------
	SantaThread(char *name, int numberOfTrips, int numberOfReindeer);

private:
	// -----------------------------------------------------------
	// FUNCTION  SantaThread::ThreadFunct
	//     Function of the Santa thread
	// FUNCTION CALLED :
	//    Delay, sprintf, _print, signal, wait, lock, unlock, Exit
	// -----------------------------------------------------------
	void ThreadFunc();
	char *name;
	int currentTrip, numberOfTrips, numberOfReindeer;
};
#endif