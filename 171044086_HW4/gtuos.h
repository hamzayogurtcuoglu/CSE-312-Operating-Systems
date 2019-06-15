#ifndef H_GTUOS
#define H_GTUOS

/*Hamza Yoğurtcuoğlu
171044086*/


#include "8080emuCPP.h"
#include <fstream>
#include "TYPES.h"
#include <vector>
#include <queue>

enum SYS_CALL{
  PRINT_B = 0x04,
  PRINT_MEM = 0x03,
  READ_B = 0x07,
  READ_MEM = 0x02,
  PRINT_STR = 0x01,
  READ_STR = 0x08,
  LOAD_EXEC = 0x05,
  PROCESS_EXIT = 0x09,
  SET_QUANTUM = 0x06,
  PRINT_WHOLE = 0x0a,
  RAND_INT =  0x0c,
  WAIT = 0x0d,   
  SIGNAL = 0x0e  
};


class processes {
  
public:
  string name; //
  int pid;
} ;


class GTUOS{  
 public:
 
   GTUOS();
  ~GTUOS();

  processes  processTable[4];
  uint64_t randInteger(CPU8080& cpu);
  uint64_t waitProcess(CPU8080& cpu);
  uint64_t signal(CPU8080& cpu);
  uint64_t handleCall(CPU8080  & cpu);
  uint64_t sysPrintB(CPU8080 & cpu);
  uint64_t sysPrintMem(CPU8080 & cpu);
  uint64_t sysReadB(CPU8080 & cpu);
  uint64_t sysReadMem(CPU8080 & cpu);
  uint64_t sysPrintStr(CPU8080 & cpu);
  uint64_t sysReadStr(CPU8080 & cpu);
  uint64_t loadExec(CPU8080& cpu);
  uint64_t processExit(CPU8080& cpu);
  uint64_t setQuantum(CPU8080& cpu);  
  uint64_t contextSwitching(CPU8080& cpu);
  void initProcessTable(std::string processName,uint16_t ppid,CPU8080& cpu);
  void pageFault(int virtualPage,CPU8080& cpu); 
 private:
    std::queue < std::pair<int,int> > readyQueue;
    std::vector < std::pair<int,int> > blockedQueue;
    short int runningProcessCount;
    short int runningProcess;
    uint16_t allProcessCount;
};

#endif
