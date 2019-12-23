// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil 
// DUE DATE : 11/1/2019                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread.cpp (your unix file name)            
// PROGRAM PURPOSE :                                           
//    A thread that either copies a value from i-1 or computes the addition     
// ----------------------------------------------------------- 
#include "thread.h"
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>

// ----------------------------------------------------------- 
// FUNCTION  _print : (function name)                          
//     Wrapper for writing to stdout                          
// PARAMETER USAGE :                                           
//    buff - string to write           
// FUNCTION CALLED :                                           
//    write, memset      
// ----------------------------------------------------------- 
void PrefixSum::_print(char* buff){
	write(1, buff, strlen(buff));
	memset(buff, 0, sizeof(buff));
}

// ----------------------------------------------------------- 
// FUNCTION  PrefixSum : (function name)                          
//     Default constructor                        
// PARAMETER USAGE :                                           
//    output - array to read and save values to
//    i - row value of current thread
//    k - column value of current thread        
// FUNCTION CALLED :    
// ----------------------------------------------------------- 
PrefixSum::PrefixSum(int** output, int i, int k)
		:
		output(output),
		i(i),
		k(k)

{
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc : (function name)                    
//    A thread function that either copies a value from i-1 or computes the addition            
// PARAMETER USAGE :                                                
// FUNCTION CALLED :    
//     memset, sprintf, _print, Exit
// ----------------------------------------------------------- 
void PrefixSum::ThreadFunc()
{
	char buff[1000];
	int gap;

	memset(buff, 0, sizeof(buff));

	sprintf(buff, "    Thread %d Created \n", k);
	_print(buff);
	gap = k - pow(2, i - 1);
	if (gap < 0)
	{
		sprintf(buff, "    Thread %d copies x[%d]\n", k, k);
		_print(buff);
		output[i][k] = output[i - 1][k];
	}else
	{
		sprintf(buff, "    Thread %d computes x[%d] + x[%d]\n", k, k, gap);
		_print(buff);
		output[i][k] = output[i - 1][k] + output[i - 1][gap];
	}
	sprintf(buff, "    Thread %d exits\n", k);
	_print(buff);
	Exit();
}