#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;	

	// This is just a sample main function, you should rewrite this file to handle problems 
	// with new multitasking and virtual memory additions.
int main (int argc, char**argv)
{
	
	ifstream input;
	input.open("input.txt"); 
	ofstream output;
	output.open("output.txt", ios::out | ios::trunc );
	int totalCycle = 0;
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1); 
	}
	int DEBUG = atoi(argv[2]);

	memory mem;
	CPU8080 theCPU(&mem);
	GTUOS	theOS;
	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);	

	do	
	{
		theCPU.Emulate8080p(DEBUG);
		if(theCPU.isSystemCall()){
			totalCycle += theOS.handleCall(theCPU,output,input);
			
		}
	}	while (!theCPU.isHalted());
	output<<endl;
	output<<"Total Cycle : "<<totalCycle;
	output.close();
	input.close();
	return 0;
}

