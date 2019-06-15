

#ifndef PAGEFAULTEXCEPTION_H
#define PAGEFAULTEXCEPTION_H


/*Hamza Yoğurtcuoğlu
171044086*/

#include <iostream>
#include <string>

class PageFault {
private:
    std::string m_error;
    int pageFrame;
public:

    PageFault(std::string error,int pageFrame) :m_error(error),pageFrame(pageFrame){}
    std::string getError() { return m_error; }
    int getPageFrame(){return pageFrame;}
};
#endif 
