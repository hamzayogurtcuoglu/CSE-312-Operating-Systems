#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"
#include "mmuVirtual.h"
#include "PageFault.h"
using namespace std;


/*Hamza Yoğurtcuoğlu
171044086*/

int main (int argc, char**argv)
{
   srand( time(NULL) );
  if (argc != 3){
    std::cerr << "Usage: prog exeFile debugOption\n";
    exit(1); 
  }
  cout<<endl;
  cout<<"NOTE : RUN TIME WILL BE '10 Second' Because of Sort Please Wait!"<<endl;
  cout<<"NOTE : Because of Disk can be get segmentation Fault TRY AGAIN Please!"<<endl;

  int DEBUG = atoi(argv[2]);
  MMUVirtual mem;
  CPU8080 theCPU(&mem);
  GTUOS	theOS;
  theCPU.setQuantum(100);
  theOS.initProcessTable(argv[1],0,theCPU);
  do{
    try{
      if(DEBUG == 2)
        getchar();    
      theCPU.Emulate8080p(DEBUG);
        if(theCPU.isSystemCall()){
  	       theOS.handleCall(theCPU);
        }
    }catch(PageFault &e){
       cout<<"Hamza"<<endl;
       theOS.pageFault(e.getPageFrame(),theCPU);
    }    
  }while(!theCPU.isHalted());
  return 0;
}
