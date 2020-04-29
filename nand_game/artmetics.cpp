#include "arthmetics.h"
#include "gates.h"
#include "utils.h"

OneBitAdderResult HalfAdder(bool inputA, bool inputB)
{
	OneBitAdderResult result = { 0 };
	result.l = Xor(inputA, inputB);
	result.h = And(inputA, inputB);
	return result;
}

OneBitAdderResult FullAdder(bool inputA, bool inputB, bool caryyIn)
{
	OneBitAdderResult result = { 0 };
	bool xorResult = Xor(inputA, inputB);
	result.l = Xor(xorResult, caryyIn);
	result.h = (
		Or(
			And(xorResult, caryyIn),
			And(inputA, inputB)
		)
		);
	return result;
}

uint16_t Bit16Adder(uint16_t inputA, uint16_t inputB, bool carryIn, bool* carryOut)
{
	Bit16Splitted splitted_a = Bit16Split(inputA);
	Bit16Splitted splitted_b = Bit16Split(inputB);

	uint16_t result = 0;

	OneBitAdderResult intermediateResult = FullAdder(splitted_a.bit[0], splitted_b.bit[0], carryIn);
	result |= intermediateResult.l;

	for (size_t i = 1; i < 16; i++)
	{
		intermediateResult = FullAdder(splitted_a.bit[i], splitted_b.bit[i], intermediateResult.h);
		result |= intermediateResult.l << i;
	}
	if (carryOut != nullptr)
	{
		*carryOut = intermediateResult.h;
	}

	return result;
}

uint16_t Bit16Increment(uint16_t input, bool* carryOut)
{
	return Bit16Adder(input, 0, 1, carryOut);
}

uint16_t Bit16Invert(uint16_t input)
{
	Bit16Splitted splittedInput = Bit16Split(input);
	uint16_t result = 0;
	for (size_t i = 0; i < 16; i++)
	{
		result |= !splittedInput.bit[i] << i;
	}
	return result;
}

uint16_t Bit16Subtract(uint16_t inputA, uint16_t inputB)
{
	return Bit16Invert(Bit16Adder(Bit16Invert(inputA), inputB, 0, nullptr));
}

bool Bit16EqualToZero(uint16_t input)
{
	// I could split input and make bitwise or on each and invert the result.
	// But I'll just check uint16_t input in C way.
	return input == 0;
}

bool Bit16LessThanZero(uint16_t input)
{
	Bit16Splitted splittedInput = Bit16Split(input);
	return splittedInput.bit[15] == 1;
}
