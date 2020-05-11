#pragma once
#include "alu.h"
#include "memory.h"

class CombinedMemory
{
private:
	Register regA;
	Register regD;
	Ram512Reg ram;
public:
	struct Value
	{
		uint16_t regA = 0;
		uint16_t regD = 0;
		uint16_t aStar = 0;
	};

	Value setNewStateAndGetResult(bool a, bool d, bool starA, uint16_t data, bool clockSignal);
};

class InstructionDecoder
{
public:
	struct Decoded
	{
		bool ci;
		bool sm;
		bool zx;
		bool nx;
		bool zy;
		bool ny;
		bool f;
		bool no;
		bool a;
		bool d;
		bool starA;
		bool gt;
		bool eq;
		bool lt;
		uint16_t word;
	};
	static Decoded decode(uint16_t input);
};

class ControlUnit
{
private:
	CombinedMemory mem;
public:
	struct Result
	{
		bool j;
		uint16_t regAValue;
	};
	Result setNewStateAndGetResult(uint16_t input, bool clockSignal);
private:
	CombinedMemory::Value prevMemValue;
};