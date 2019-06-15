#ifndef MMU_HM_HEADER
#define MMU_HM_HEADER

#include <cstdlib>
#include "memory.h"
#include "TYPES.h"
#include <vector>
#include <queue>
#include <fstream> 
#include "PageFault.h"
/*Hamza Yoğurtcuoğlu
171044086*/


class MMUVirtual: public Memory {
public:
    MMUVirtual();
    ~MMUVirtual();
    virtual uint8_t & at(uint32_t ind);
    virtual uint8_t & physicalAt(uint32_t ind);
    uint16_t getBaseRegister() const { return baseRegister;}
    uint16_t getLimitRegister() const { return limitRegister;}
    void setBaseResister(uint16_t base) { this->baseRegister = base;}
    void setLimitRegister(uint16_t limit) {this->limitRegister = limit;}
    int findPid(){return getBaseRegister()==0 ? 0 : (getBaseRegister() - 1792)/512+1;}
    void PageHandle(uint8_t pid,int memPageIndex, int diskPageIndex,int rePlacement);
    int virtualAddressToPhysicalAddress(int virtualAddress,uint8_t pid);
private:
    Page_t pageTable[4][16]; // 16KB virtual space
    uint8_t * mem;
    uint8_t * disk;
    uint16_t baseRegister;
    uint16_t limitRegister;
    int currentPageIndex; 
    int rePlacement;
    ofstream pageTabletxt;
    std::queue<int> myqueue;
};
#endif