#pragma once
#include <cstdint>
#include <vector>

struct Bit16Splitted
{
	bool bit[16];
};

Bit16Splitted Bit16Split(int16_t);
int16_t Bit16Join(Bit16Splitted);
bool Bit1Selector(bool inputSwitch, bool data0, bool data1);
void Bit1Switch(bool canalSwitch, bool data, std::vector<bool> &outputCanals);
void Bit1Switch4Way(std::vector<bool> canalSwitch, bool data, std::vector<bool>& outputCanals);

int16_t Bit16Selector(bool dataSwitch, uint16_t data0, uint16_t data1);
