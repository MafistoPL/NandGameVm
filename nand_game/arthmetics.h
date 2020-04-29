#pragma once
#include <cstdint>

struct OneBitAdderResult
{
	bool h;
	bool l;
};

OneBitAdderResult HalfAdder(bool inputA, bool inputB);
OneBitAdderResult FullAdder(bool inputA, bool inputB, bool carryIn);
uint16_t Bit16Adder(uint16_t inputA, uint16_t inputB, bool carryIn, bool *caryyOut);
uint16_t Bit16Increment(uint16_t input, bool *carryOut);
uint16_t Bit16Invert(uint16_t input);
uint16_t Bit16Subtract(uint16_t inputA, uint16_t inputB);
bool Bit16EqualToZero(uint16_t input);
bool Bit16LessThanZero(uint16_t input);