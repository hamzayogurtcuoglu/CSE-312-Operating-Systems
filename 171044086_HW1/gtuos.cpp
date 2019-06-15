#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

GTUOS :: GTUOS(){
	/*constructor initialize cycles to 0*/
	this->cycles = 0;
}


uint64_t GTUOS::handleCall(const CPU8080 & cpu,ofstream & output,ifstream & input){
	
	uint8_t regA = cpu.state->a;
	
	this->cycles = 0;
	switch(regA){
		case PRINT_B  :{

			output <<(int)(cpu.state->b);

			this->cycles = PRINT_B_CYCLES;

		} break;
		case PRINT_MEM: {


			//find the address to get the context
			uint16_t address = cpu.state->b << 8 | cpu.state->c;
			output << (int)cpu.memory->at(address);
			output << (int)cpu.memory->at(address+1);


			this->cycles = PRINT_MEMB_CYCLES;

		}break;
		case READ_B   : {

			int value ;
			input >> value;

			//if the number is bigger than 256 than make the content of the reg B 0.
			cpu.state->b = value >= 0 && value <= 255 ? value : 0 ;

			//then give the info about that.
			if(cpu.state->b == 0)
				cerr << "The register called B is assigned to 0 because of BOUND_ERROR"<<endl;

			this->cycles = READ_B_CYCLES;

		} break;
		case READ_MEM : {

			//output << "--------READ_MEM--------"<<endl;

			int value ;
			input >> value;

			//find the address to get the context
			uint16_t address = cpu.state->b << 8 | cpu.state->c;
			//if the number is bigger than 256 than make the content of the address(BC) 0.
			cpu.memory->at(address) = value >= 0 && value <= 255 ? value : 0 ;
			
			if(cpu.memory->at(address)== 0)
				cerr << "The memory address BC is assigned to 0 because of BOUND_ERROR"<<endl;
			
			this->cycles =  READ_MEM_CYCLES;

		} break;
		case PRINT_STR:{

			uint8_t i =  0;
			//find the address to get the context
			uint16_t address = cpu.state->b << 8 | cpu.state->c;
			while(cpu.memory->at((address + i) )!= '\0'){
				output <<cpu.memory->at(address+ i++);
				this->cycles += 10;	
			}

		}  break;
		case READ_STR : {

			string str = " 	";
			input >> str;
			int size = str.size();
			//find the address to get the context
			uint16_t address = cpu.state->b << 8 | cpu.state->c;
			if (str.size() >= 0 && str.size() <= 255 )
			{
				cpu.state->d = str.size(); 
			}else{
				output<<'\n'<<"You entered upper character 255"<<endl;
			}	
				 
			int i = 0;
			for( ; i < size ; i++){
				cpu.memory->at(address + i) = str[i];
				this->cycles = this->cycles + 10;
			}
			cpu.memory->at(address+ i) = '\n'; //add newline character
			cpu.memory->at(address+ i) = '\0'; //add null character

		} break;

		default: cerr << "ERROR! Not system call!" << endl;  this->cycles = 0 ; break;
	}
	return (this->cycles);	
}



