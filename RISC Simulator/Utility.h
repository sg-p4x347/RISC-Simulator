#pragma once
#include "pch.h"
template<typename Bus>
inline Bus Multiplex(uint8_t input, uint8_t address)
{
	// bit shift by [address] number of bytes to the left
	return (dynamic_cast<Bus>(input) << address * 8);

}
template<typename Bus>
inline uint8_t Multiplex(Bus input, uint8_t address)
{
	// bit shift by [address] number of bytes to the right
	return static_cast<uint8_t>(input >> address * 8);

}