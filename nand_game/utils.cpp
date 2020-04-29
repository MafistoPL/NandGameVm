#include "utils.h"
#include "gates.h"

Bit16Splitted Bit16Split(int16_t input)
{
	Bit16Splitted result;
	int16_t mask = 0x1;
	for (size_t i = 0; i < 16; i++)
	{
		result.bit[i] = input & mask;
		mask <<= 1;
	}
	return result;
}

int16_t Bit16Join(Bit16Splitted input)
{
	int16_t result = 0;
	for (size_t i = 0; i < 16; i++)
	{
		result |= input.bit[i];
	}
	return result;
}

bool Bit1Selector(bool inputA, bool inputB, bool inputSwitch)
{
	return Or(And(Not(inputSwitch), inputA), And(inputSwitch, inputB));
}

void Bit1Switch(bool s, bool d, bool* c0, bool* c1)
{
	*c0 = And(Not(s), d);
	*c1 = And(s, d);
}
