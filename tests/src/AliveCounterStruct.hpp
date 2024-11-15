#pragma once

#include <cstddef>

struct AliveCounterStruct
{
	std::size_t aliveCount = 0;
	std::size_t copyCount = 0;
	std::size_t moveCount = 0;
};