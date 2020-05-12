#pragma once
#include <cinttypes>
#include <string.h>
#include <vector>
#include "gates.h"

class Latch
{
private:
	bool leftNandEarlierOutput = 0;
	bool rightNandEarlierOutput = 1;

public:
	bool setNewStateAndGetResult(bool set, bool data);
};

class DataFlipFlop
{
private:
	static const size_t Number_Of_Gates = 10;
	bool prevOutputOfGates[Number_Of_Gates] = {
		// Initial state of circuit
		0, // 0
		1, // 1
		1, // 2
		1, // 3
		1, // 4
		0, // 5
		0, // 6
		1, // 7
		0, // 8
		1  // 9
	};
public:
	bool setNewStateAndGetResult(bool set, bool data, bool clockSignal);
};

class Register
{
private:
	static const size_t Bit_Count = 16;
	DataFlipFlop dff[Bit_Count];
public:
	uint16_t setNewStateAndGetResult(bool set, uint16_t data, bool clockSignal);
};

class Counter
{
private:
	Register reg;
	uint16_t prevRegOutput = 0;
public:
	uint16_t setNewStateAndGetResult(bool set, uint16_t x, bool clockSignal);
};

class Ram2Reg
{
private:
	Register ram[2];
public:
	uint16_t setNewStateAndGetResult(bool address, bool set, uint16_t input, bool clockSignal);
};

class Ram8Reg
{
private:
	Register ram[8];
public:
	uint16_t setNewStateAndGetResult(std::vector<bool> address, bool set, uint16_t input, bool clockSignel);
};

class Ram64Reg
{
private:
	Ram8Reg ram[8];
public:
	uint16_t setNewStateAndGetResult(std::vector<bool> address, bool set, uint16_t input, bool clockSignel);
};

class Ram512Reg
{
private:
	Ram64Reg ram[8];
public:
	uint16_t setNewStateAndGetResult(std::vector<bool> address, bool set, uint16_t input, bool clockSignal);
	uint16_t setNewStateAndGetResult(uint16_t address, bool set, uint16_t input, bool clockSignal);
};
