#include "memory.h"
#include "arthmetics.h"
#include "utils.h"

bool Latch::setNewStateAndGetResult(bool st, bool d)
{
	bool left = Nand(st, d);
	bool right = Nand(st, Not(d));

	bool leftNandOutput = Nand(left, rightNandEarlierOutput);
	bool rightNandOutput = Nand(right, leftNandOutput);

	rightNandEarlierOutput = rightNandOutput;
	leftNandOutput = Nand(left, rightNandOutput);

	return leftNandOutput;
}

bool DataFlipFlop::setNewStateAndGetResult(bool st, bool d, bool cl)
{
	bool currOutputOfGates[Number_Of_Gates] = { 0 };
	bool outputChanged = false;

	size_t loopCounter = 0;
	do
	{
		if (loopCounter++ > 1000)
		{
			throw "Undefined state";
		}
		outputChanged = false;
		currOutputOfGates[0] = Nand(prevOutputOfGates[2], prevOutputOfGates[1]);
		currOutputOfGates[1] = Nand(prevOutputOfGates[0], prevOutputOfGates[3]);
		currOutputOfGates[2] = Nand(prevOutputOfGates[5], d);
		currOutputOfGates[3] = Nand(prevOutputOfGates[4], prevOutputOfGates[5]);
		currOutputOfGates[4] = Not(d);
		currOutputOfGates[5] = And(st, prevOutputOfGates[6]);
		currOutputOfGates[6] = And(cl, prevOutputOfGates[7]);
		currOutputOfGates[7] = Not(prevOutputOfGates[8]);
		currOutputOfGates[8] = Not(prevOutputOfGates[9]);
		currOutputOfGates[9] = Not(cl);

		for (size_t gateNr = 0; gateNr < Number_Of_Gates; gateNr++)
		{
			if (prevOutputOfGates[gateNr] != currOutputOfGates[gateNr])
			{
				prevOutputOfGates[gateNr] = currOutputOfGates[gateNr];
				outputChanged = true;
			}
		}
	} while (outputChanged == true);
	return currOutputOfGates[0];
}

uint16_t Register::setNewStateAndGetResult(bool set, uint16_t data, bool clockSignal)
{
	uint16_t result = 0;
	Bit16Splitted splittedData = Bit16Split(data);

	for (size_t i = 0; i < Register::Bit_Count; i++)
	{
		result |= dff[i].setNewStateAndGetResult(set, splittedData.bit[i], clockSignal) << i;
	}

	return result;
}

uint16_t Counter::setNewStateAndGetResult(bool set, uint16_t x, bool clockSignal)
{
	uint16_t prevRegIncremented = Bit16Increment(prevRegOutput, nullptr);
	prevRegOutput =
		reg.setNewStateAndGetResult(1, Bit16Selector(set, prevRegIncremented, x), clockSignal);
	return prevRegOutput;
}

uint16_t Ram2Reg::setNewStateAndGetResult(bool address, bool set, uint16_t input, bool clockSignal)
{
	std::vector<bool> outputCanals(2);
	Bit1Switch(address, set, outputCanals);
	return Bit16Selector(
		address,
		ram[0].setNewStateAndGetResult(outputCanals[0], input, clockSignal),
		ram[1].setNewStateAndGetResult(outputCanals[1], input, clockSignal)
	);
}

uint16_t Ram8Reg::setNewStateAndGetResult(std::vector<bool> address, bool set, uint16_t input, bool clockSignal)
{
	if (address.size() != 3)
	{
		throw "Ram8Reg: address bus must have a width of 3 bits.";
	}

	std::vector<bool> enable(8);
	std::vector<uint16_t> data;
	Bit1Switch8Way(address, set, enable);
	for (size_t i = 0; i < 8; i++)
	{
		data.push_back(ram[i].setNewStateAndGetResult(enable[i], input, clockSignal));
	}

	return Bit16Selector8Way(address, data);
}

uint16_t Ram64Reg::setNewStateAndGetResult(std::vector<bool> address, bool set, uint16_t input, bool clockSignal)
{
	if (address.size() < 6)
	{
		throw "Ram64Reg: address bus must have at least a width of 6 bits.";
	}
	std::vector<bool> ram8RegAddr(address.begin(), address.begin() + 3);
	std::vector<bool> rem8RegInnerAddr(address.begin() + 3, address.begin() + 6);

	std::vector<bool> enable(8);
	std::vector<uint16_t> data;
	Bit1Switch8Way(ram8RegAddr, set, enable);
	for (size_t i = 0; i < 8; i++)
	{
		data.push_back(ram[i].setNewStateAndGetResult(rem8RegInnerAddr, enable[i], input, clockSignal));
	}

	return Bit16Selector8Way(ram8RegAddr, data);
}

uint16_t Ram512Reg::setNewStateAndGetResult(std::vector<bool> address, bool set, uint16_t input, bool clockSignal)
{
	if (address.size() < 9)
	{
		throw "Ram512Reg: address bus must have at least a width of 9 bits.";
	}
	std::vector<bool> ram64RegAddr(address.begin(), address.begin() + 3);
	std::vector<bool> ram64RegInnerAddr(address.begin() + 3, address.begin() + 9);

	std::vector<bool> enable(8);
	std::vector<uint16_t> data;
	Bit1Switch8Way(ram64RegAddr, set, enable);
	for (size_t i = 0; i < 8; i++)
	{
		data.push_back(ram[i].setNewStateAndGetResult(ram64RegInnerAddr, enable[i], input, clockSignal));
	}

	return Bit16Selector8Way(ram64RegAddr, data);
}
