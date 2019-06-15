
/*Hamza Yoğurtcuoğlu
171044086*/

#ifndef GENERAL_TYPES
#define GENERAL_TYPES

#include "memoryBase.h"
#include "8080emuCPP.h"

#define SIZE1K 1024
#define SIZE8K 8192
#define PAGE_TABLE_SIZE 16 // 16*1KB = 16KB Virtual Address Space
#define process2 "Sum" 
#define process1 "Prime" 
#define process3 "Sort" 

using namespace std;

typedef struct {
	bool modifiedBit;
	bool referencedBit;
	bool presentAbsent;
	int frame; //
} Page_t;


#endif