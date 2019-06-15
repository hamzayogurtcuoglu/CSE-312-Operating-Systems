


#include <iostream>
#include "mmuVirtual.h"
#include "TYPES.h"

/*Hamza Yoğurtcuoğlu
171044086*/


MMUVirtual::MMUVirtual() {
    
    pageTabletxt.open("pagetable.txt");
    mem = (uint8_t*) malloc(0x2000);  // Memory 8KB
    disk = (uint8_t*) malloc(0x2000); // Disk  8KB
    baseRegister = 0;
    limitRegister = 0;
    currentPageIndex=0;
    for (int i = 0; i < 4; ++i)       //pageTable Inıtialization there is 4 process.
         for (int j = 0; j < 16; ++j) // Each Process has 16 pages
        {    
             pageTable[i][j].modifiedBit = false;  // Each situation will be vice versa 
             pageTable[i][j].referencedBit = false;  
             pageTable[i][j].presentAbsent = false;
             pageTable[i][j].frame = -1;     
        }   
    for (int i = 0; i < SIZE1K; ++i) 
        mem[i] = disk[i];    
    for (int i = 0; i < SIZE8K; ++i)
        myqueue.push ((i*7)%16);
    limitRegister = SIZE8K;  // LimitRegister will be 8K Because of 
}

MMUVirtual::~MMUVirtual() {
    free(disk);
    free(mem);
    pageTabletxt.close();
}

uint8_t & MMUVirtual::at(uint32_t ind) {
  int frame;
  int processNum = findPid();
  uint16_t lastProcess = 0x00cffe;
  uint8_t pid = Memory::at(lastProcess);
  int pageNum = ind / SIZE1K;
  int physicalAddress = virtualAddressToPhysicalAddress(ind,pageNum);
  int offset = physicalAddress % SIZE1K;
  if (pageTable[processNum][pageNum].presentAbsent) {
    frame = pageTable[processNum][pageNum].frame;
    if (unsigned(pid)!=0&&!myqueue.empty()){
        rePlacement = myqueue.front();
        PageHandle(pid,frame*SIZE1K,pageNum*SIZE1K,rePlacement);       
        myqueue.pop();
    }
  } else {
    if (currentPageIndex + 1> PAGE_TABLE_SIZE)
      currentPageIndex = 0;
    ++currentPageIndex;
    PageHandle(pid,currentPageIndex*SIZE1K,pageNum*SIZE1K,rePlacement);
    pageTable[processNum][pageNum].presentAbsent=1;
    pageTable[processNum][pageNum].frame=currentPageIndex;
    frame = currentPageIndex;
  }
    return Memory::at(physicalAddress);
}


int MMUVirtual::virtualAddressToPhysicalAddress(int virtualAddress,uint8_t pid){
    int physicalAddress = virtualAddress;
    int virtualPage = virtualAddress >> 10;
    int mask = 0;
    mask = mask | 0b1111111111;
    if(pageTable[unsigned(pid)][virtualPage].presentAbsent=0) {
        pageTable[unsigned(pid)][virtualPage].referencedBit=true;
        physicalAddress = pageTable[unsigned(pid)][virtualPage].frame;
        physicalAddress = physicalAddress << 10;
        physicalAddress = physicalAddress|(virtualAddress & mask);
        return physicalAddress;
    }else{
        pageTable[unsigned(pid)][virtualPage].presentAbsent=true;
    }
    return physicalAddress;
}


uint8_t & MMUVirtual::physicalAt(uint32_t ind) {
    return Memory::physicalAt(ind);
}



void MMUVirtual::PageHandle(uint8_t pid,int memPageIndex, int diskPageIndex,int rePlacement){
     pageTabletxt<<"Current_PID : "<<unsigned(pid)<<" , Virtual_Address : "<<diskPageIndex%SIZE8K<<
             ", Physical_Address :  "<<abs(memPageIndex%SIZE8K)<<" , Page_To_Be_Replaced :  "<<rePlacement<<endl;   
}
