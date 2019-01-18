#pragma once
#include "pch.h"
#include <bitset>
#include <fstream>
template<typename Bus>
class ROM {
public:
	ROM(size_t instructionSize = 0);
public:
	void Load(string fileName);
	const vector<bool> & Read(int lineAddress);
private:
	size_t m_instructionSize;
	vector<vector<bool>> m_memory;
};

template<typename Bus>
inline ROM<Bus>::ROM(size_t instructionSize) : m_instructionSize(instructionSize)
{
}

template<typename Bus>
inline void ROM<Bus>::Load(string fileName)
{
	m_memory.clear();
	std::ifstream ifs(fileName, std::ios::binary);
	int bufferSize = dynamic_cast<int>(std::ceilf(dynamic_cast<float>(m_instructionSize) / 8.f));
	while (ifs.is_open() && !ifs.eof()) {
		char * buffer = new char[bufferSize];
		ifs.read(buffer, bufferSize);
		vector<bool> instruction;
		for (int byteIndex = 0; byteIndex < bufferSize; byteIndex++) {
			for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
				instruction.push_back((buffer[byteIndex] >> bitIndex) & 1);
			}
		}
		m_memory.push_back(instruction);
	}
}

template<typename Bus>
inline const vector<bool> & ROM<Bus>::Read(int lineAddress)
{
	return m_memory[lineAddress];
}
