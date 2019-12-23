// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 06/12/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : boat-monitor.h (your unix file name)            
// PROGRAM PURPOSE :                                           
//    Header file for boat-monitor, contains definitions        
// ----------------------------------------------------------- 

#ifndef BOAT_MONITOR_H
#define BOAT_MONITOR_H

#include "ThreadClass.h"
#include <stdio.h>
#include <string.h>

#define AVAILABLE 0
#define READY 1
#define LOADING 2
#define LOADED 3
#define SAILING 4
#define EMPTY 5

#define CANNIBAL 0
#define MISSIONARY 1

void _print(char[100]);

class BoatMonitor : public Monitor {
public:
    BoatMonitor(int numberOfTrips);
// ----------------------------------------------------------- 
// FUNCTION  BoatReady : (function name)                          
//     Kicks off the loading of the boat and transportion                                   
// FUNCTION CALLED :                                           
//    MonitorBegin, MonitorEnd, Signal, sprintf, _print        
// -----------------------------------------------------------
    void BoatReady();
// ----------------------------------------------------------- 
// FUNCTION  BoatDone : (function name)                          
//     Kicks off the readying up of the boat again              
// FUNCTION CALLED :                                           
//    MonitorBegin, MonitorEnd, Signal        
// -----------------------------------------------------------
    void BoatDone();
// ----------------------------------------------------------- 
// FUNCTION  getCurrentTrip : (function name)                          
//     return the current trip number                                   
// -----------------------------------------------------------
    int getCurrentTrip();
// ----------------------------------------------------------- 
// FUNCTION  CannibalArrives : (function name)                          
//     Queues up the cannibal to board the boat   
// PARAMETER USAGE :                                           
//    index of the thread              
// FUNCTION CALLED :                                           
//    MonitorBegin, MonitorEnd, Wait   
// -----------------------------------------------------------
    void CannibalArrives(int identifier);
// ----------------------------------------------------------- 
// FUNCTION  MissionaryArrives : (function name)                          
//     Queues up the Missionary to board the boat   
// PARAMETER USAGE :                                           
//    index of the thread              
// FUNCTION CALLED :                                           
//    MonitorBegin, MonitorEnd, Wait   
// -----------------------------------------------------------
    void MissionaryArrives(int identifier);
// ----------------------------------------------------------- 
// FUNCTION  boatAvailable : (function name)                          
//      Returns if the boat is in the empty state 
// -----------------------------------------------------------
    int boatAvailable();
// ----------------------------------------------------------- 
// FUNCTION  boatLoaded : (function name)                          
//      Returns if the boat is in the loaded state 
// -----------------------------------------------------------
    int boatLoaded();
// ----------------------------------------------------------- 
// FUNCTION  passengerList : (function name)                          
//      returns a list of passengers on the boat
// FUNCTION CALLED :                                           
//    malloc, memset, sprintf, strcat 
// -----------------------------------------------------------
    char* passengerList();

private:
    int numberOfTrips;
    int currentTrip;
    int cannibalsWaiting;
    int missionariesWaiting;
    int passengersOnBoat;
    int boatState;
    int passengerId[3];
    int passengerType[3];

    time_t t;

    Condition* MissionaryQueue;
    Condition* CannibalQueue;
    Condition* ExitBoat;
};

#endif