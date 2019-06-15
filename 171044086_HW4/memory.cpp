#include <iostream>
#include "memory.h"
using namespace std;

/*Hamza Yoğurtcuoğlu
171044086*/

Memory::Memory() {
	mem = (uint8_t*) malloc(0x2000); // Memory 8KB
	baseRegister = 0;
	limitRegister = 0;
}

uint8_t & Memory::at(uint32_t ind) {
	return mem[ind + baseRegister];
}