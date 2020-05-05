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
		result |= input.bit[i] << i;
	}
	return result;
}

bool Bit1Selector(bool inputSwitch, bool data0, bool data1)
{
	return Or(And(Not(inputSwitch), data0), And(inputSwitch, data1));
}

void Bit1Switch(bool canalSwitch, bool data, std::vector<bool>& outputCanals)
{
	if (outputCanals.size() != 2)
	{
		throw "In function Bit1Switch size of outputCanals must be 2.";
	}
	outputCanals[0] = And(Not(canalSwitch), data);
	outputCanals[1] = And(canalSwitch, data);
}

int16_t Bit16Selector(bool dataSwitch, uint16_t data0, uint16_t data1)
{
	Bit16Splitted splittedData0 = Bit16Split(data0);
	Bit16Splitted splittedData1 = Bit16Split(data1);
	Bit16Splitted result;
	for (size_t i = 0; i < 16; i++)
	{
		result.bit[i] = Bit1Selector(dataSwitch, splittedData0.bit[i], splittedData1.bit[i]);
	}
	return Bit16Join(result);
}
