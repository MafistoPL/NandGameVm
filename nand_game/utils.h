#pragma once
#include <cstdint>

struct Bit16Splitted
{
	bool bit[16];
};

Bit16Splitted Bit16Split(int16_t);
int16_t Bit16Join(Bit16Splitted);
bool Bit1Selector(bool inputSwitch, bool data0, bool data1);
void Bit1Switch(bool canalSwitch, bool data, bool* canal0, bool* canal1);

int16_t Bit16Selector(bool dataSwitch, uint16_t data0, uint16_t data1);
