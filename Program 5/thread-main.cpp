// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 06/12/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread-main.cpp (your unix file name)            
// PROGRAM PURPOSE :                                           
//    Entry point of program       
// ----------------------------------------------------------- 
#ifndef THREAD_MAIN_CPP
#define THREAD_MAIN_CPP

#include "ThreadClass.h"
#include "boat-monitor.h"
#include "thread.h"
#include <stdio.h>

BoatMonitor* monitor;
// ----------------------------------------------------------- 
// FUNCTION  main : (function name)                          
//    Entry point of program                               
// FUNCTION CALLED :                                           
//    new, printf, atoi, Begin     
// -----------------------------------------------------------
int main(int argc, char* argv[]){

    int numberOfCannibals;
    int numberOfMissionaries;
    int numberOfTrips;
    int i;

    numberOfCannibals = 8;
    numberOfMissionaries = 8;
    numberOfTrips = 5;

    if (argc < 4){
        printf("useage ./prog5 <cannibals> <missionaries> <boat trips>\n");
    }

    if(atoi(argv[1]) != 0){
        numberOfCannibals = atoi(argv[1]);
    }

    if(atoi(argv[2]) != 0){
        numberOfMissionaries = atoi(argv[2]);
    }

    if(atoi(argv[3]) != 0){
        numberOfTrips = atoi(argv[3]);
    }

    monitor = new BoatMonitor(numberOfTrips);

    BoatThread* boatThread = new BoatThread(numberOfTrips);
    CannibalThread* cannibalThread[numberOfCannibals];
    MissionaryThread* missionaryThread[numberOfMissionaries];

    boatThread->Begin();

    for (i = 0; i < numberOfCannibals; i++){
        cannibalThread[i] = new CannibalThread(i);
        cannibalThread[i]->Begin();
    }

    for(i = 0; i < numberOfMissionaries; i++){
        missionaryThread[i] = new MissionaryThread(i);
        missionaryThread[i]->Begin();
    }

    boatThread->Join();
    exit(0);
}

#endif