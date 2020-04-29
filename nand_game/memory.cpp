#include "memory.h"
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
		result = dff[i].setNewStateAndGetResult(set, splittedData.bit[i], clockSignal);
	}

	return result;
}
