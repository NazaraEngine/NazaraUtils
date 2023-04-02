#include <NazaraUtils/Prerequisites.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

// This is a compilation test

float sqrtCheck(float x)
{
	if NAZARA_UNLIKELY(x < 0)
	{
		throw std::runtime_error("square root of negative value");
	}

	return std::sqrt(x);
}

bool logEnabled = false;

void debugLog()
{
	if NAZARA_LIKELY(!logEnabled)
	{
		return;
	}

	std::cout << "Debug log" << std::endl;
}
