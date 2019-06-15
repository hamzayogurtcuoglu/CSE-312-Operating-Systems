#include <iostream>
#include "memory.h"
using namespace std;

Memory::Memory(uint64_t size) {
	//	cout<<"\nBBBBBBBBB   "<<size<<endl;
	mem = (uint8_t*) calloc(size, sizeof(uint8_t));
	baseRegister = 0;
	limitRegister = 0;
}

uint8_t & Memory::at(uint32_t ind) {
	//cout<<"\nAAAAAA   "<<ind<<" baseRegister  "<<baseRegister<<" limitRegister "<<limitRegister<<endl;
	return mem[ind + baseRegister];
}