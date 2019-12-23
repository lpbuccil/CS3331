// ----------------------------------------------------------- 
// NAME : Lucas Buccilli                    User ID: lpbuccil 
// DUE DATE : 11/1/2019                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread-main.cpp (your unix file name)            
// PROGRAM PURPOSE :                                           
//    Concurrently solve for a sum prefix of a sequence of numbers        
// ----------------------------------------------------------- 
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>  
#include <string.h>


// ----------------------------------------------------------- 
// FUNCTION  _print : (function name)                          
//     Wrapper for writing to stdout                          
// PARAMETER USAGE :                                           
//    buff - string to write           
// FUNCTION CALLED :                                           
//    write, memset      
// ----------------------------------------------------------- 
void _print(char* buff)
{
	write(1, buff, strlen(buff));
	memset(buff, 0, sizeof(buff));
}

using namespace std;
int** output;

// ----------------------------------------------------------- 
// FUNCTION  main : (function name)                          
//     Entry point to program                        
// PARAMETER USAGE :                                           
//    argv, argc        
// FUNCTION CALLED :                                           
//    printf, memset, _print, sprintf, free, malloc      
// ----------------------------------------------------------- 
int main(int argv, char* argc[])
{
	int numberOfElements;
	int i, k, j, rows;
	vector< vector<PrefixSum*> > threadId;
	char buff[1000];
	memset(buff, 0, sizeof(buff));
	
	cin >> numberOfElements;

	
	printf("Number of input data = %d\n", numberOfElements);
	rows = log2(numberOfElements) + 1;

	int **output = new int*[rows];
	//read in each number and store the value into the first slot of the output array
	//Create a vectore to store the thread ID
	for (i = 0; i < rows; i++)
	{
		output[i] = new int[numberOfElements];
		vector<PrefixSum*> temp;
		threadId.push_back(temp);
	}

	printf("Input Array\n");
	for (i = 0; i < numberOfElements; i++)
	{
		cin >> output[0][i];
		printf("    %d", output[0][i]);
	}
	printf("\n");

	for(i = 1; i < rows; i++)
	{
		sprintf(buff, "Run %d:\n", i);
		_print(buff);

		//create a thread and store the id into a vector
		//start the thread
		for (k = 0; k < numberOfElements; k++)
		{
			threadId.at(i).push_back(new PrefixSum(output, i, k));
			threadId.at(i).at(k)->Begin();		
		}

		//join to the main thread to prevent race condition
		for (k = 0; k < numberOfElements; k++)
		{
			threadId.at(i).at(k)->Join();	
		}

		sprintf(buff, "Result after run %d:\n", i);
		for (k = 0; k < numberOfElements; k++)
		{
			sprintf(buff + strlen(buff), "    %d", output[i][k]);
		}
		sprintf(buff + strlen(buff), "\n");
		_print(buff);
	}

	//formatting
	if (rows == 1)
	{
		printf("Final result after run %d:\n", rows);
	}else 
	{
		printf("Final result after run %d:\n", rows - 1);
	}

	for (i = 0; i < numberOfElements; i++)
	{
		printf("    %d", output[rows - 1][i]);
	}
	printf("\n");

	for(i = 0; i < rows; i++)
	{
		free(output[i]);
	}
	free(output);

}