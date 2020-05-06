#pragma once
#include <cstdint>
#include <vector>

struct Bit16Splitted
{
	bool bit[16];
};

Bit16Splitted Bit16Split(uint16_t);
uint16_t Bit16Join(Bit16Splitted);
bool Bit1Selector(bool inputSwitch, bool data0, bool data1);
void Bit1Switch(bool canalSwitch, bool data, std::vector<bool> &outputCanals);
void Bit1Switch4Way(std::vector<bool> canalSwitch, bool data, std::vector<bool>& outputCanals);
void Bit1Switch8Way(std::vector<bool> canalSwitch, bool data, std::vector<bool>& outputCanals);

uint16_t Bit16Selector(bool dataSwitch, uint16_t data0, uint16_t data1);
uint16_t Bit16Selector4Way(std::vector<bool> dataSwitch, std::vector<uint16_t> data);
uint16_t Bit16Selector8Way(std::vector<bool> dataSwitch, std::vector<uint16_t> data);
