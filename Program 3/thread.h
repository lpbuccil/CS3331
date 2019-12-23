// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil 
// DUE DATE : 11/1/2019                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread.h (your unix file name)            
// PROGRAM PURPOSE :                                           
//    Header for thread.cpp file     
// ----------------------------------------------------------- 
#ifndef THREAD_H
#define THREAD_H
#include "ThreadClass.h"

class PrefixSum : public Thread
{
public:
	PrefixSum(int** output, int i, int k);

private:
	int** output;
	int i;
	int k;
	void _print(char*);
	void ThreadFunc();
};
#endif