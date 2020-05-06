#include "utils.h"
#include "gates.h"

Bit16Splitted Bit16Split(uint16_t input)
{
	Bit16Splitted result;
	uint16_t mask = 0x1;
	for (size_t i = 0; i < 16; i++)
	{
		result.bit[i] = input & mask;
		mask <<= 1;
	}
	return result;
}

uint16_t Bit16Join(Bit16Splitted input)
{
	uint16_t result = 0;
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
	outputCanals.clear();

	outputCanals.push_back(And(Not(canalSwitch), data));
	outputCanals.push_back(And(canalSwitch, data));
}

void Bit1Switch4Way(std::vector<bool> canalSwitch, bool data, std::vector<bool>& outputCanals)
{
	if (canalSwitch.size() != 2)
	{
		throw "In funciton Bit1Switch4Way size of canalSwitch must be 2.";
	}
	outputCanals.clear();

	std::vector<bool> intermediateOutputCanals;
	std::vector<bool> sliceOfOutput;
	Bit1Switch(canalSwitch[1], data, intermediateOutputCanals);

	Bit1Switch(canalSwitch[0], intermediateOutputCanals[0], sliceOfOutput);
	outputCanals.insert(outputCanals.end(), sliceOfOutput.begin(), sliceOfOutput.end());

	Bit1Switch(canalSwitch[0], intermediateOutputCanals[1], sliceOfOutput);
	outputCanals.insert(outputCanals.end(), sliceOfOutput.begin(), sliceOfOutput.end());
}

void Bit1Switch8Way(std::vector<bool> canalSwitch, bool data, std::vector<bool>& outputCanals)
{
	if (canalSwitch.size() != 3)
	{
		throw "In funciton Bit1Switch8Way size of canalSwitch must be 3.";
	}
	outputCanals.clear();

	std::vector<bool> outputOfDMux;
	std::vector<bool> outputOfDMux4;
	std::vector<bool> intermediateSwitch = { canalSwitch[0], canalSwitch[1] };

	Bit1Switch(canalSwitch[2], data, outputOfDMux);
	Bit1Switch4Way(intermediateSwitch, outputOfDMux[0], outputOfDMux4);
	outputCanals.insert(outputCanals.end(), outputOfDMux4.begin(), outputOfDMux4.end());

	Bit1Switch4Way(intermediateSwitch, outputOfDMux[1], outputOfDMux4);
	outputCanals.insert(outputCanals.end(), outputOfDMux4.begin(), outputOfDMux4.end());
}

uint16_t Bit16Selector(bool dataSwitch, uint16_t data0, uint16_t data1)
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

uint16_t Bit16Selector4Way(std::vector<bool> dataSwitch, std::vector<uint16_t> data)
{
	if (dataSwitch.size() != 2)
	{
		throw "In function Bit16Selector4Way, vector of dataSwitch should contain 2 boolean values.";
	}
	if (data.size() != 4)
	{
		throw "In function Bit16Selector4Way, vector of data should contain 4 items.";
	}
	uint16_t c1 = Bit16Selector(dataSwitch[0], data[0], data[1]);
	uint16_t c2 = Bit16Selector(dataSwitch[0], data[2], data[3]);
	return Bit16Selector(dataSwitch[1], c1, c2);
}

uint16_t Bit16Selector8Way(std::vector<bool> dataSwitch, std::vector<uint16_t> data)
{
	if (dataSwitch.size() != 3)
	{
		throw "In function Bit16Selector8Way, vector of dataSwitch should contain 3 boolean values.";
	}
	if (data.size() != 8)
	{
		throw "In function Bit16Selector4Way, vector of data should contain 8 items.";
	}
	std::vector<bool> intermediateDataSwitch = { dataSwitch[0], dataSwitch[1] };
	std::vector<uint16_t> v1(data.begin(), data.begin() + 4);
	std::vector<uint16_t> v2(data.begin() + 4, data.end());
	uint16_t c1 = Bit16Selector4Way(intermediateDataSwitch, v1);
	uint16_t c2 = Bit16Selector4Way(intermediateDataSwitch, v2);
	return Bit16Selector(dataSwitch[2], c1, c2);
}
