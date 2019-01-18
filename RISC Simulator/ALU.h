#pragma once
#include "pch.h"
#include "GOTO.h"
enum AluOpCode {
	Passthrough,
	Add,
	AND,
	OR,
	NOT,
	XOR,
	LeftShift,
	RightShift
};
template<typename Bus>
class ALU {
public:
	
	ALU(Bus & bus, GOTO & pGoto,size_t registerCount) : m_goto(pGoto), m_bus(bus), m_registers(registerCount,0) {}
public:
	void Operation(AluOpCode operation);
	void ReadA(uint8_t address);
	void ReadB(uint8_t address);
	void WriteA(uint8_t address);
	void WriteB(uint8_t address);
private:
	GOTO & m_goto;
	Bus & m_bus;
	Bus m_A;
	Bus m_B;
	vector<Bus> m_Aregisters;
	vector<Bus> m_Bregisters;
	
};

template<typename Bus>
inline void ALU<Bus>::Operation(AluOpCode operation)
{
	Bus result = 0;
	switch (operation) {
	case Passthrough: result = m_A; break;
	case Add: result = m_A + m_B; break;
	case AND: result = m_A & m_B; break;
	case OR: result = m_A | m_B; break;
	case NOT: result = ~m_A; break;
	case XOR: result = m_A ^ m_B; break;
	case LeftShift: result = m_A << 1; break;
	case RightShift: result = m_A >> 1; break;
	}
	m_bus |= result;
}

template<typename Bus>
inline void ALU<Bus>::ReadA(uint8_t address)
{
	m_A = m_Aregisters[address];
}

template<typename Bus>
inline void ALU<Bus>::ReadB(uint8_t address)
{
	m_B = m_Bregisters[address];
}

template<typename Bus>
inline void ALU<Bus>::WriteA(uint8_t address)
{
	m_Aregisters[address] = m_bus;
}

template<typename Bus>
inline void ALU<Bus>::WriteB(uint8_t address)
{
	m_Bregisters[address] = m_bus;
}
