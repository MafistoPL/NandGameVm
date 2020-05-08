#pragma once
#include "utils.h"
#include "arthmetics.h"

class UnaryALU
{
public:
	static uint16_t Compute(bool zeroFlag, bool negateOutputFlag, uint16_t input);
};

class ALU
{
public:
	static uint16_t Compute(std::vector<bool> flags, uint16_t x, uint16_t y);
};
