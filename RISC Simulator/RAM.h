#pragma once
#include "pch.h"
#include "Utility.h"
template<typename Bus>
class RAM
{
public:
	RAM(Bus & bus, size_t size);
public:
	void SetAddress();
	void Read(uint8_t byteAddress);
	void Write(uint8_t byteAddress);
private:
	Bus & m_bus;
	Bus m_address;
	vector<uint8_t> m_memory;
};

template<typename Bus>
inline RAM<Bus>::RAM(Bus & bus, size_t size) : m_bus(bus), m_address(0), m_memory(size,0)
{
}

template<typename Bus>
inline void RAM<Bus>::SetAddress()
{
	m_address = m_bus;
}

template<typename Bus>
inline void RAM<Bus>::Read(uint8_t byteAddress)
{
	m_bus |= Multiplex<Bus>(m_memory[m_address],byteAddress);
}

template<typename Bus>
inline void RAM<Bus>::Write(uint8_t byteAddress)
{
	m_memory[m_address] = Multiplex<Bus>(m_bus,byteAddress);
}
