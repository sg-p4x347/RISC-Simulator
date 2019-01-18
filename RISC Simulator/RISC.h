#pragma once
#include "pch.h"
#include "ROM.h"
#include "RAM.h"
#include "ALU.h"
#include "GOTO.h"
enum Instruction {
	// GOTO ---------
	Goto,
	SetPC,
	// Input --------
	ReadInputBuffer,
	// Output -------
	WriteOutputBuffer,
	// ROM ----------
	Data,
	// ALU ----------
	Aselector,
	Bselector,
	ReadA,
	ReadB,
	WriteA,
	WriteB,
	AluOperation,
	// RAM ----------
	SetAddress,
	Read,
	Write,
	ByteSelector // selects which byte out of the bus to use in read/write operations
};
template<typename Bus>
class RISC
{
public:
	RISC(size_t ramSize, size_t registerCount);
public:
	void Run(string fileName);
private:
	ROM<Bus> m_rom;
	RAM<Bus> m_ram;
	ALU<Bus> m_alu;
	GOTO m_goto;
	Bus m_bus;
	Bus m_inputBuffer;
	Bus m_outputBuffer;
	bool m_run;
	map<Instruction, size_t> m_instructionSizes;
	map<Instruction, size_t> m_instructionPositions;

private:
	void MapInstruction(Instruction instruction, size_t size);
	template<typename T>
	T DecodeInstruction(const vector<bool> & instructionSet, Instruction instruction);
	template<typename T>
	T Cast(const vector<bool> & instruction, int start, int count);
};

template<typename Bus>
inline RISC<Bus>::RISC(size_t ramSize, size_t registerCount) : m_goto(),m_bus(0), m_ram(m_bus, ramSize), m_alu(m_goto,m_bus, registerCount) {
	MapInstruction(Goto, 4);
	MapInstruction(SetPC, 1);
	MapInstruction(ReadInputBuffer, 1);
	MapInstruction(WriteOutputBuffer, 1);
	MapInstruction(Data, sizeof(Bus));
	MapInstruction(Aselector, 3);
	MapInstruction(Bselector, 3);
	MapInstruction(ReadA, 1);
	MapInstruction(ReadB, 1);
	MapInstruction(WriteA, 1);
	MapInstruction(WriteB, 1);
	MapInstruction(AluOperation, 3);
	MapInstruction(SetAddress, 1);
	MapInstruction(Read, 1);
	MapInstruction(Write, 1);
	MapInstruction(ByteSelector, 3);

}

template<typename Bus>
inline void RISC<Bus>::Run(string fileName)
{
	m_rom.Load(fileName);
	m_run = true;
	Bus programAddress = 1;
	while (programAddress) {
		const vector<bool> & instructionRegister = m_rom.Read(programAddress);
		// execute Bus write instructions first
		// clear
		m_bus = 0;
		m_goto.Clear();
		// ALU
		uint8_t aSelector = DecodeInstruction<uint8_t>(Aselector);
		uint8_t bSelector = DecodeInstruction < uint8_t(Bselector);
		if (DecodeInstruction<bool>(instructionRegister,ReadA)) {
			m_alu.ReadA(aSelector);
		}
		if (DecodeInstruction<bool>(instructionRegister,ReadB)) {
			m_alu.ReadB(bSelector);
		}
		m_alu.Operation(dynamic_cast<AluOpCode>(DecodeInstruction<uint8_t>(instructionRegister,AluOperation)));
		// Data
		Bus data = DecodeInstruction<Bus>(instructionRegister, Data);
		m_bus |= data;
		// RAM
		uint8_t byteSelector = DecodeInstruction<uint8_t>(instructionRegister, ByteSelector);
		if (DecodeInstruction<bool>(instructionRegister, Read)) {
			m_ram.Read(byteSelector)
		}
		if (DecodeInstruction<bool>(instructionRegister, Read)) {
			m_ram.Read(byteSelector)
		}
		// Input
		if (DecodeInstruction<bool>(instructionRegister, ReadInputBuffer)) {
			m_bus |= m_inputBuffer;
		}
		// Execute Bus read instructions second

		// ALU
		if (DecodeInstruction<bool>(instructionRegister, WriteA)) {
			m_alu.WriteA(aSelector);
		}
		if (DecodeInstruction<bool>(instructionRegister, WriteB)) {
			m_alu.WriteB(bSelector);
		}
		// Program Counter
		if (DecodeInstruction<bool>(instructionRegister, SetPC)) {
			programAddress = m_bus;
		}
		// RAM
		if (DecodeInstruction<bool>(instructionRegister, Write)) {
			m_ram.Write(byteSelector);
		}
		// Output
		if (DecodeInstruction<bool>(instructionRegister, WriteOutputBuffer)) {
			m_outputBuffer = m_bus;
		}
		// GOTO
		// if condition is true, address is taken from data
		ConditionalFlags condition = DecodeInstruction<ConditionalFlags>(instructionRegister, Goto);
		if (m_goto.Check(condition)) {
			programAddress = data;
		}
		else {
			programAddress++;
		}

	}
}

template<typename Bus>
inline void RISC<Bus>::MapInstruction(Instruction instruction, size_t size)
{
	static size_t pos = 0;
	m_instructionPositions.insert(std::make_pair(instruction, pos));
	m_instructionSizes.insert(std::make_pair(instruction, size));
	pos += size;
}

template<typename Bus>
template<typename T>
inline T RISC<Bus>::DecodeInstruction(const vector<bool> & instructionSet, Instruction instruction)
{
	return Cast<T>(instructionSet, m_instructionPositions[instruction], m_instructionSizes[instruction]);
}

template<typename Bus>
template<typename T>
inline T RISC<Bus>::Cast(const vector<bool>& instruction, int start, int count)
{
	T value;
	for (int index = start; index < start + count; index++) {
		value = value << 1;
		value |= instruction[index];
	}
	return value;
}
