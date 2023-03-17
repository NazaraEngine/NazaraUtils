#include <Nazara/Prerequisites.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

// This is a compilation test

float sqrtCheck(float x)
{
	NAZARA_IF_UNLIKELY(x < 0)
	{
		throw std::runtime_error("square root of negative value");
	}

	return std::sqrt(x);
}

bool logEnabled = false;

void debugLog(float x)
{
	NAZARA_IF_LIKELY(!logEnabled)
	{
		return;
	}

	std::cout << "Debug log" << std::endl;
}
