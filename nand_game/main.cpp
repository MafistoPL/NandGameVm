#include <iostream>
#include "cpu.h"

#define GATE Xor;

int main()
{
	/*std::vector<uint16_t> program = {
		0xEAA0,
		0xEFC8,
		0xEDE0,
		0xEFC8,
		0xEC10,
		0xE3A0,
		0xFC10,
		0xEDE0,
		0xF090,
		0xEDE0,
		0xE308,
		0xEC10,
		0x0005,
		0xE301,
		0x000E,
		0xEA87
	};
	Computer cpu(program);

	int index = 0;
	while (true) 
	{
		int debug = 0;
		debug++;
		cpu.clockPulse();
		index++;
	}*/
	
	int16_t mem[65536];
	int16_t a = 0, d = 1;
	mem[a] = mem[d] = 1;
	while (d > 0)
	{
		mem[d + 1] = mem[a] + mem[d];
		a++; d++;
	}

	return 0;
}