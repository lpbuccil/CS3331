// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 06/12/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread.h (your unix file name)            
// PROGRAM PURPOSE :                                           
//    Header file for thread, contains definitions        
// ----------------------------------------------------------- 
#ifndef THREAD_H
#define THREAD_H

#include "ThreadClass.h"
#include "boat-monitor.h"

extern BoatMonitor* monitor;

class BoatThread : public Thread
{
   public:
        BoatThread(int numberOfTrips);
    private:
        void ThreadFunc();
        int numberOfTrips;
        char* name; 
};

class MissionaryThread : public Thread
{
    public:
        MissionaryThread(int identifier);
    private:
        void ThreadFunc();
        int identifier;
        char* name;
};

class CannibalThread : public Thread
{
    public:
        CannibalThread(int identifier);
    private:
        void ThreadFunc();
        int identifier;
        char* name;
};



#endif