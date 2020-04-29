#pragma once
#include <cstdint>

struct Bit16Splitted
{
	bool bit[16];
};

Bit16Splitted Bit16Split(int16_t);
int16_t Bit16Join(Bit16Splitted);
bool Bit1Selector(bool inputA, bool inputB, bool inputSwitch);
void Bit1Switch(bool s, bool d, bool* c0, bool* c1);
