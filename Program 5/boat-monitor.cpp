// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil
// DUE DATE : 06/12/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : boat-monitor.cpp (your unix file name)            
// PROGRAM PURPOSE :                                           
//     implementation of boat monitor, comments are in header   
// ----------------------------------------------------------- 
#ifndef BOAT_MONITOR_CPP
#define BOAT_MONITOR_CPP

#include "boat-monitor.h"

void _print(char msg[100])
{
    write(1, msg, strlen(msg));
    memset(msg, '\0', sizeof(msg));
}

BoatMonitor::BoatMonitor(int numberOfTrips)
    :
    Monitor("boat-monitor", HOARE),
    numberOfTrips(numberOfTrips),
    currentTrip(0),
    cannibalsWaiting(0),
    missionariesWaiting(0),
    passengersOnBoat(0),
    boatState(EMPTY),
    MissionaryQueue(new Condition("MissionaryQueue")),
    CannibalQueue(new Condition("CannibalQueue")),
    ExitBoat(new Condition("ExitBoat"))
{
    int i;
    for (i = 0; i < 3; i++){
        passengerId[i] = -1;
        passengerType[i] = -1;
    }
    srand((unsigned) time(&t));
}

void BoatMonitor::BoatReady(){
    MonitorBegin();
    char buff[100];
    

    boatState = READY;

    /*adding rand make this more random. Removing it allows multip missionaries and cannibals to queue up and always trigger the third option*/
    if (missionariesWaiting > 2 && (cannibalsWaiting == 0 || rand() % 2)){
        
        MissionaryQueue->Signal();
        MissionaryQueue->Signal();
        MissionaryQueue->Signal();

        sprintf(buff, "MONITOR (%d): Three missionaries are selected (%d, %d, %d)\n", currentTrip + 1, passengerId[0] + 1, passengerId[1] + 1, passengerId[2] + 1);
        _print(buff);
    }else if (cannibalsWaiting > 2 && (missionariesWaiting < 2 || rand() % 2)){
        
        CannibalQueue->Signal();
        CannibalQueue->Signal();
        CannibalQueue->Signal();

         sprintf(buff, "MONITOR (%d): Three cannibals are selected (%d, %d, %d)\n", currentTrip + 1, passengerId[0] + 1, passengerId[1] + 1, passengerId[2] + 1);
        _print(buff);

    }else{
        MissionaryQueue->Signal();
        MissionaryQueue->Signal();
        CannibalQueue->Signal();

        int cannable, missionary1, missionary2;

        if (passengerType[0] == CANNIBAL){
            cannable = 0;
            missionary1 = 1;
            missionary2 = 2;
        }else if (passengerType[1] == CANNIBAL){
            cannable = 1;
            missionary1 = 0;
            missionary2 = 2;
        }else{
            cannable = 2;
            missionary1 = 0;
            missionary2 = 1;
        }
        sprintf(buff, "MONITOR (%d): Two missionaries (%d, %d) and one cannibal are selected (%d)\n", currentTrip + 1, passengerId[missionary1] + 1, passengerId[missionary2] + 1, passengerId[cannable] + 1);
        _print(buff);
    }

    boatState = LOADED;

    MonitorEnd();
}

void BoatMonitor::BoatDone(){
    MonitorBegin();

    char buff[100];
    boatState = EMPTY;
    ExitBoat->Signal();
    ExitBoat->Signal();
    ExitBoat->Signal();


    currentTrip++;
    
    MonitorEnd();
}

void BoatMonitor::CannibalArrives(int identifier){
    MonitorBegin();
    cannibalsWaiting++;
    if (boatState == EMPTY && (cannibalsWaiting > 2 || missionariesWaiting > 1)){
        boatState = READY;
    }
    CannibalQueue->Wait();
    passengerId[passengersOnBoat] = identifier;
    passengerType[passengersOnBoat] = CANNIBAL;
    passengersOnBoat++;
    if (passengersOnBoat == 3){
        boatState = LOADED;
    }
    ExitBoat->Wait();
    cannibalsWaiting--;
    passengersOnBoat--;
    passengerId[passengersOnBoat] = -1;
    passengerType[passengersOnBoat] = -1;

    if (passengersOnBoat == 0){
        boatState = EMPTY;
    }
    MonitorEnd();
}

void BoatMonitor::MissionaryArrives(int identifier){
    MonitorBegin();
    missionariesWaiting++;
    if (boatState == EMPTY && (missionariesWaiting > 2 || cannibalsWaiting > 0)){
        boatState = READY;
    }
    MissionaryQueue->Wait();
    passengerId[passengersOnBoat] = identifier;
    passengerType[passengersOnBoat] = MISSIONARY;
    passengersOnBoat++;
    if (passengersOnBoat == 3){
        boatState = LOADED;
    }
    ExitBoat->Wait();
    missionariesWaiting--;
    passengersOnBoat--;
    if (passengersOnBoat == 0){
        boatState = EMPTY;
    }
    MonitorEnd();
}

int BoatMonitor::boatAvailable(){
    return (boatState == READY || (passengersOnBoat == 0  && boatState == EMPTY && missionariesWaiting > 2 && cannibalsWaiting > 1));
}
int BoatMonitor::boatLoaded(){
    return boatState == LOADED;
}

int BoatMonitor::getCurrentTrip(){
    return currentTrip;
}

char* BoatMonitor::passengerList(){
    char* list = (char*) malloc(50);
    char num[10];
    memset(list, '\0', sizeof(list));

    if (passengerType[0] == CANNIBAL){
        strcat(list, "c");
    }else {
        strcat(list, "m");
    }
    sprintf(num,"%d", passengerId[0] + 1);
    strcat(list, num);
    strcat(list, ", ");

    if (passengerType[1] == CANNIBAL){
        strcat(list, "c");
    }else {
        strcat(list, "m");
    }
    sprintf(num,"%d", passengerId[1] + 1);
    strcat(list, num);
    strcat(list, ", ");

    if (passengerType[2] == CANNIBAL){
        strcat(list, "c");
    }else {
        strcat(list, "m");
    }
    sprintf(num,"%d", passengerId[2] + 1);
    strcat(list, num);
    return list;
}

#endif