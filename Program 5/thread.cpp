// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 06/12/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread.cpp (your unix file name)            
// PROGRAM PURPOSE :                                           
//     implementation of thread logic, comments are in header   
// ----------------------------------------------------------- 
#ifndef THREAD_CPP
#define THREAD_CPP

#include "thread.h"
#include <string.h>
#include <stdio.h>

BoatThread::BoatThread(int numberOfTrips)
    : numberOfTrips(numberOfTrips)
{
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Boat" << endl;
    name = "***** Boat";
}

MissionaryThread::MissionaryThread(int identifier)
    : identifier(identifier)
{
    name = new char[100];
    int i;

    ThreadName.seekp(0, ios::beg);
    ThreadName << "Missionary" << identifier;

    memset(name, '\0', 100);
    for (i = 0; i < identifier && i < 85; i++)
    {
        strcat(name, " ");
    }
    strcat(name, "Missionary ");
    char number[10];
    sprintf(number, "%d", identifier + 1);
    strcat(name, number);
}

CannibalThread::CannibalThread(int identifier)
    : identifier(identifier)
{
    name = new char[100];
    int i;

    ThreadName.seekp(0, ios::beg);
    ThreadName << "Cannibal" << identifier;

    memset(name, '\0', 100);
    for (i = 0; i < identifier && i < 85; i++)
    {
        strcat(name, " ");
    }
    strcat(name, "Cannibal ");
    char number[10];
    sprintf(number, "%d", identifier + 1);
    strcat(name, number);
}

void BoatThread::ThreadFunc()
{
    char buff[100];
    sprintf(buff, "***** BOAT thread starts\n");
    _print(buff);

    while (monitor->getCurrentTrip() < numberOfTrips)
    {
        Thread::Delay();
        if (monitor->boatAvailable()){
            sprintf(buff, "***** boat is ready\n");
            _print(buff);
            monitor->BoatReady();
        }
        Thread::Delay();
        if (monitor->boatLoaded())
        {
            char* list = monitor->passengerList();
            sprintf(buff, "%s load (%i): Passenger list (%s)\n", name, monitor->getCurrentTrip() + 1, list);
            _print(buff);
            free(list);
            monitor->BoatDone();
        }
        Thread::Delay();
    }
    sprintf(buff, "MONITOR: %d crosses have been made.\n", monitor->getCurrentTrip());
    _print(buff);
    sprintf(buff, "MONITOR: This river cross is closed indefinitely for renovation.\n");
    _print(buff);
    Exit();
}

void CannibalThread::ThreadFunc(){
    char buff[100];
    sprintf(buff, "%s thread starts\n", name);
    _print(buff);
    while(1){
        Thread::Delay();
        sprintf(buff, "%s arrives\n", name);
         _print(buff);
         monitor->CannibalArrives(identifier);
        Thread::Delay();
    }
}

void MissionaryThread::ThreadFunc(){
    char buff[100];
    sprintf(buff, "%s thread starts\n", name);
    _print(buff);
    while(1){
        Thread::Delay();
        sprintf(buff, "%s arrives\n", name);
         _print(buff);
         monitor->MissionaryArrives(identifier);
        Thread::Delay();
    }
}

#endif