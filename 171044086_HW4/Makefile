exe:gtuos.o 8080emu.o main.o memory.o mmuVirtual.o 
	g++ -std=c++11 gtuos.o 8080emu.o main.o memory.o mmuVirtual.o  -o exe

gtuos.o:gtuos.cpp
	g++ -c gtuos.cpp

8080emu.o:8080emu.cpp
	g++ -c 8080emu.cpp

main.o:main.cpp
	g++ -c main.cpp

memory.o:memory.cpp
	g++ -c memory.cpp

mmuVirtual.o:mmuVirtual.cpp
	g++ -c mmuVirtual.cpp

clean:
	rm *.o exe
