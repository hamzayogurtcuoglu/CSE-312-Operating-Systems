
/*Hamza Yoğurtcuoğlu
171044086*/

#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"
#include <string.h>
#include <fstream> 

using namespace std;

#define MAILBOX_ID 1
#define CYCLE 10


int a = 0;
 ofstream systemOut;
 ofstream mailbox;
 ofstream locallist;
 ofstream output;

GTUOS::GTUOS(){
    locallist.open("locallist.txt");
    mailbox.open("mailbox.txt");
    output.open("output.txt");
    systemOut.open("system.txt");
    runningProcessCount=0;
    runningProcess=-1;
    allProcessCount=1;
}

GTUOS::~GTUOS(){
  /*close files*/
  mailbox.close();
  locallist.close();  
  systemOut.close();
  output.close();
  remove("locallist.txt"); 
  remove("mailbox.txt");

}

/*
  Handle the operating system calls
*/
uint64_t GTUOS::handleCall(CPU8080& cpu){
  //output << "system" << endl;
  uint16_t cycles;

  int c = 0;
  if(1) {
    switch(cpu.state->a){
    case PRINT_B:
      cycles = sysPrintB(cpu);
      break;
    case PRINT_MEM:
      cycles = sysPrintMem(cpu);    
      break;
    case READ_B:
      cycles = sysReadB(cpu);
      break;
    case READ_MEM:
      cycles = sysReadMem(cpu);
      break;
    case PRINT_STR:
      cycles = sysPrintStr(cpu);
      break;
    case READ_STR:
      cycles = sysReadStr(cpu);
      break;
    case LOAD_EXEC:
      cycles = loadExec(cpu);
      break;
    case PROCESS_EXIT:
      cycles = processExit(cpu);
      break;
    case SET_QUANTUM:
  break;
    case PRINT_WHOLE:
      cycles = contextSwitching(cpu);
      break;
    case 11:      

       for(int i = 0 ; i < 0x10000 ; ++i){
      // for(int i = 53248 ; i < 53280 ; ++i){
  if(i % 32 == 0){
    printf("\n");
    printf("%d - ",c++);
  }

  printf("%3d ",cpu.memory->at(i));
      }
      printf("\n");
      break;
    case RAND_INT: 
      cycles = randInteger(cpu);
      break;
    case WAIT: 
      cycles = waitProcess(cpu);
      break;
    case SIGNAL: 
      cycles = signal(cpu);
      break;  
    default:

      break;
    }
  }else{
    output << "Not found input.txt or output.txt" << endl;
  }
  return cycles;
}

uint64_t GTUOS::randInteger(CPU8080& cpu){

   uint8_t random = 1 + rand() % 255; //get random byte
   
   cpu.state->b = random;
   
   output << "Random byte is generated to reg B -> "<< (int)random <<endl;

   cpu.scheduler_timer += (CYCLE * 6);

  return CYCLE*6; 
}
uint64_t GTUOS::waitProcess(CPU8080& cpu){
  
  int interrupt = 0;

  uint16_t processStartAddr = (uint16_t)1 +(((uint16_t)cpu.state->b << 8) | cpu.state->c);


  uint8_t SemFull = (int)cpu.memory->at(processStartAddr);

  mailbox<<"----------MAILBOX---------"<<endl;

  int mailboxSize = 400;
  while(mailboxSize){

    int element = (int)cpu.memory->at(processStartAddr);

    if (element!=0)
    {
      mailbox<<unsigned(element)<<endl;
    }
    processStartAddr +=1;
    mailboxSize--;    
  }
  if (interrupt == 1)
  {
    cpu.dispatchScheduler();
  }

  cpu.scheduler_timer += CYCLE*20;
  return CYCLE*20;

}

uint64_t GTUOS::signal(CPU8080& cpu){

  int interrupt = 1;

  uint16_t processStartAddr = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

  uint8_t SemFull = (int)cpu.memory->at(processStartAddr);

  processStartAddr += (uint16_t)1;
  

  int element = (int)cpu.memory->at(processStartAddr);      

      locallist<<"----------LOCALLIST---------"<<endl;
      processStartAddr++;
      SemFull--;
    

  while((int)SemFull!=0){

     int element = (int)cpu.memory->at(processStartAddr);
     
    if (element!=0)
    {
      locallist<<unsigned(element)<<endl;
    }
    processStartAddr++;
    SemFull--;
  }
  if (interrupt == 0)
  {
    cpu.dispatchScheduler();
  }

  cpu.scheduler_timer += CYCLE*20;

  return CYCLE*20;

}

/*
 * Called to set the quantum time of scheduling
 */
uint64_t GTUOS::setQuantum(CPU8080& cpu){
  
   uint8_t quantum = (int)cpu.state->b; //get random byte

  cpu.setQuantum(quantum);
  return 0;
}


uint64_t GTUOS::processExit(CPU8080& cpu){

  uint16_t baseProcessAddr = 0x00500;
  uint16_t processLen = 512;

  uint16_t ptableStart = 0x0d000;
  uint16_t ptableLen = 256;

  uint16_t ptableAddr = ptableStart;
  
  uint16_t currProcessAddr = ((Memory*)(cpu.memory))->getBaseRegister();
  uint16_t tempProcessAddr = currProcessAddr;

  uint16_t schedulerAddr = 0x00040;
  
  int i = 0;
  while(tempProcessAddr != baseProcessAddr){
    tempProcessAddr -=processLen;
    i++;
  }

  while(i > 0){
    ptableAddr += ptableLen;
    i--;
  }

  uint16_t lastProcess = 0x0cffe;
  uint16_t processCount = 0x0cfff;
  
  ((Memory*)(cpu.memory))->setBaseRegister(0);
  memset(&cpu.memory->at(currProcessAddr),0,processLen);
  memset(&cpu.memory->at(ptableAddr),0,ptableLen);

  //cpu.memory->at(lastProcess) = cpu.memory->at(lastProcess) - 1;
  cpu.memory->at(processCount) = cpu.memory->at(processCount) - 1;
  cpu.state->pc = schedulerAddr; //go back to scheduler
  cpu.scheduler_timer += (CYCLE * 8);
  
  return (CYCLE * 8);
}

/*
 * Loads a new process to the given address 
 **/
uint64_t GTUOS::loadExec(CPU8080& cpu){
  uint16_t fileNameAddr;
  uint16_t processStartAddr;
  
  char* fileName = (char* ) malloc(sizeof(uint8_t) * 64);

  fileNameAddr = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
  int i = 0;

  while(cpu.memory->at(fileNameAddr) != (uint8_t) 0){
    fileName[i] = cpu.memory->at(fileNameAddr);
    fileNameAddr++;
    i++;
  }
  fileName[i] = 0;

  processStartAddr = ((uint16_t)cpu.state->h << 8) | cpu.state->l;
  
  output<<fileName<<" "<<processStartAddr<<endl;

  cpu.scheduler_timer += (CYCLE * 10);
  string processName(fileName);
  processTable[runningProcessCount].name = processName;
  processTable[runningProcessCount].pid = runningProcessCount;
  cpu.ReadFileIntoMemoryAt(fileName, processStartAddr);
  readyQueue.push(std::make_pair(runningProcessCount,processTable[runningProcessCount].pid));
  ++runningProcessCount;
  ++allProcessCount;
   
  return (CYCLE * 10);
}

/*
  Print the contents of register B
*/
uint64_t GTUOS::sysPrintB( CPU8080& cpu){
  //output << "-";
  output <<(int)cpu.state->b;
  output<<endl;

  //printf("%d",(int)cpu.state->b);
  cpu.scheduler_timer += CYCLE;
  
  return CYCLE;
}
/*
  Print the content of memory pointed by B and C
  Calculate start address first then get the block
*/
uint64_t GTUOS::sysPrintMem( CPU8080& cpu){
  uint16_t fileNameAddr;
  fileNameAddr = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
  printf("%d \n",(int)cpu.memory->at(fileNameAddr));
  cpu.scheduler_timer += CYCLE;
  return CYCLE;
}

/*
  Read an integer and put it to B
*/
uint64_t GTUOS::sysReadB( CPU8080& cpu){
  uint16_t num;
  cin >> num;
  cpu.state->b = num;
  cpu.scheduler_timer += CYCLE;
  return CYCLE;
}

/*
  Read an integer and put it to memory address pointed
  by B and C
*/
uint64_t GTUOS::sysReadMem( CPU8080& cpu){
    int readVal;
    uint16_t realValue;

    cin >> readVal;
    realValue = (uint16_t) readVal;

    if (readVal < 0 || readVal > 65535){
      output << "Error! Bigger than 16 bits not accepted" << endl;
    }else{
      uint8_t leastSign = (uint8_t) realValue;
      uint8_t mostSign = (uint8_t) (realValue >> 8);
      cpu.state->c = leastSign;
      cpu.state->b = mostSign;
    }
  cpu.scheduler_timer += (CYCLE);
    
    return CYCLE;
}

/*
  Print the null terminated string
  whose start address is stored cin B and C
*/
uint64_t GTUOS::sysPrintStr( CPU8080& cpu){
  uint16_t fileNameAddr;
  fileNameAddr = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
  
  while(cpu.memory->at(fileNameAddr) != (uint8_t) 0){
    output << cpu.memory->at(fileNameAddr);
    fileNameAddr++;
  }
  output<<endl;
  cpu.scheduler_timer += (CYCLE);

  return CYCLE;
}
/*
  Read string and put it to memory address
  pointed by B and C
*/
uint64_t GTUOS::sysReadStr( CPU8080& cpu){
  uint16_t fileNameAddr;
  string str;
  cin >> str;
  fileNameAddr = (((uint16_t)cpu.state->b) << 8) | cpu.state->c;
  for(int i = 0 ; i < (signed) str.length() ; ++i){
    cpu.memory->at(fileNameAddr) = str[i];
    fileNameAddr++;
  }
  cpu.memory->at(fileNameAddr) = (uint8_t)'\0'; 
  cpu.scheduler_timer += (CYCLE);
  
  return CYCLE;
}


uint64_t GTUOS::contextSwitching(CPU8080& cpu){

    uint16_t lastProcess = 0x00cffe;
    uint8_t pid = cpu.memory->at(lastProcess);
    int pidd = pid;
    switch(unsigned(pidd)){
        case 1:
            systemOut<<"Current_PID : "<<unsigned(pidd)<<" , Current_PNAME : "<<process1<<
             ", Next_PID :  "<<pid+1<<" , Next_PNAME :  "<<process2<<endl;
            break;
        case 2:
            systemOut<<"Current_PID : "<<unsigned(pidd)<<" , Current_PNAME : "<<process2<<
             ", Next_PID :  "<<pid+1<<" , Next_PNAME :  "<<process3<<endl;
            break;
        case 3:
            systemOut<<"Current_PID : "<<unsigned(pidd)<<" , Current_PNAME : "<<process3<<
             ", Next_PID : 1"<<" , Next_PNAME :  "<<process1<<endl;
            break;
    }  
  return CYCLE;
}
void GTUOS::initProcessTable(std::string processName,uint16_t ppid,CPU8080& cpu) {
    if (runningProcessCount >= 4)
        return;
    else {
        processTable[0].name = processName;
        processTable[0].pid = 0;
        cpu.ReadFileIntoMemoryAt(processName.c_str(), 0x0000);
        readyQueue.push(std::make_pair(0,processTable[0].pid));
        ++runningProcessCount;
        ++allProcessCount;
    }
}

void GTUOS::pageFault(int virtualPage,CPU8080& cpu) {
    int index=virtualPage<<10;
    int pageFrame;
    uint8_t *bufferMem,*bufferhardDisk;
    Page_t temp;
    if(processTable[runningProcess].pid==-1){
        pageFrame=0;
    }
    else if(processTable[runningProcess].pid!=-1){

        while(1){
            if(temp.referencedBit){
                temp.referencedBit= false;
            }
            else{
                if(temp.modifiedBit){

                    memcpy(bufferhardDisk,bufferMem, 1024);
                    temp.modifiedBit=false;
                }
                temp.presentAbsent=false;
                break;
            }
        }
    }
}
