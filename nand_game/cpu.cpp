#include "cpu.h"

CombinedMemory::Value CombinedMemory::setNewStateAndGetResult(bool a, bool d, bool starA, uint16_t data, bool clockSignal)
{
	Value retVal;
	retVal.regA = regA.setNewStateAndGetResult(a, data, clockSignal);
	retVal.regD = regD.setNewStateAndGetResult(d, data, clockSignal);
	retVal.aStar = ram.setNewStateAndGetResult(retVal.regA, starA, data, clockSignal);

	return retVal;
}

InstructionDecoder::Decoded InstructionDecoder::decode(uint16_t input)
{
	Decoded decoded;
	Bit16Splitted splittedInput = Bit16Split(input);
	Bit16Splitted flags = Bit16Split(Bit16Selector(splittedInput.bit[15], 0, input));
	decoded.word = Bit16Selector(splittedInput.bit[15], input, 0);
	decoded.ci = flags.bit[15];
	decoded.sm = flags.bit[12];
	decoded.zx = flags.bit[11];
	decoded.nx = flags.bit[10];
	decoded.zy = flags.bit[9];
	decoded.ny = flags.bit[8];
	decoded.f = flags.bit[7];
	decoded.no = flags.bit[6];
	decoded.a = Or(flags.bit[5], Not(splittedInput.bit[15]));
	decoded.d = flags.bit[4];
	decoded.starA = flags.bit[3];
	decoded.gt = flags.bit[2];
	decoded.eq = flags.bit[1];
	decoded.lt = flags.bit[0];
	return decoded;
}

ControlUnit::Result ControlUnit::setNewStateAndGetResult(uint16_t input, bool clockSignal)
{
	Result result;
	
	InstructionDecoder::Decoded dIn = InstructionDecoder::decode(input);
	std::vector<bool> aluFlags = { dIn.no, dIn.f, dIn.ny, dIn.zy, dIn.nx, dIn.zx };
	uint16_t aluSource = Bit16Selector(dIn.sm, prevMemValue.regA, prevMemValue.aStar);
	uint16_t aluOutput = ALU::Compute(aluFlags, prevMemValue.regD, aluSource);
	result.j = Condition(dIn.lt, dIn.eq, dIn.gt, aluOutput);
	
	uint16_t memoryInput = Bit16Selector(dIn.ci, dIn.word, aluOutput);
	prevMemValue = mem.setNewStateAndGetResult(dIn.a, dIn.d, dIn.starA, memoryInput, clockSignal);
	result.regAValue = prevMemValue.regA;
	
	return result;
}

ProgramEngine::ProgramEngine(std::vector<uint16_t> program)
{
	for (uint16_t i = 0; i < program.size(); ++i)
	{
		rom.setNewStateAndGetResult(i, 1, program[i], 0);
		rom.setNewStateAndGetResult(i, 1, program[i], 1);
	}
}

uint16_t ProgramEngine::getNextInstruction(bool jump, uint16_t address, bool clockSignal)
{
	uint16_t romAddress = counter.setNewStateAndGetResult(jump, address, clockSignal);
	return rom.setNewStateAndGetResult(romAddress, 0, 0, 0);
}
