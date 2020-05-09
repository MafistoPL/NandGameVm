#include "alu.h"

uint16_t UnaryALU::Compute(bool zeroFlag, bool negateOutputFlag, uint16_t input)
{
	uint16_t intermediateResult = Bit16Selector(zeroFlag, input, 0);
	return Bit16Selector(negateOutputFlag, intermediateResult, ~intermediateResult);
}

uint16_t ALU::Compute(std::vector<bool> flags, uint16_t x, uint16_t y)
{
	if (flags.size() != 6)
	{
		throw "The flag register size must be 6.";
	}
	uint16_t zeroX = Bit16Selector(flags[5], x, 0);
	uint16_t negateX = Bit16Selector(flags[4], zeroX, ~zeroX);
	uint16_t zeroY = Bit16Selector(flags[3], y, 0);
	uint16_t negateY = Bit16Selector(flags[2], zeroY, ~zeroY);
	uint16_t function = Bit16Selector(flags[1],
		negateX & negateY,
		Bit16Adder(negateX, negateY, 0, nullptr)
	);
	return Bit16Selector(flags[0], function, ~function);
}

bool Condition(std::vector<bool> inputFlags, uint16_t x)
{
	bool isNeg = Bit16LessThanZero(x);
	bool isZero = Bit16EqualToZero(x);
	return Or(
		Or(
			And(
				inputFlags[0],
				isNeg
			),
			And(
				inputFlags[1],
				isZero
			)
		),
		And(
			inputFlags[2],
			Not(Or(isNeg, isZero))
		)
	);
}
