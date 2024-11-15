#pragma once

#include <NazaraUtils/PrivateImpl.hpp>
#include "AliveCounterStruct.hpp"

class AliveCounter;

class AliveCounterWrapperDynAlloc
{
	public:
		AliveCounterWrapperDynAlloc(AliveCounterStruct* counter, int value);
		AliveCounterWrapperDynAlloc(const AliveCounterWrapperDynAlloc&);
		AliveCounterWrapperDynAlloc(AliveCounterWrapperDynAlloc&&);
		~AliveCounterWrapperDynAlloc();

		void Test(int expectedValue);

		AliveCounterWrapperDynAlloc& operator=(const AliveCounterWrapperDynAlloc&);
		AliveCounterWrapperDynAlloc& operator=(AliveCounterWrapperDynAlloc&&);

	private:
		Nz::PrivateImpl<AliveCounter> m_privateCounter;
};

class AliveCounterWrapperInPlace
{
	public:
		AliveCounterWrapperInPlace(AliveCounterStruct* counter, int value);
		AliveCounterWrapperInPlace(const AliveCounterWrapperInPlace&);
		AliveCounterWrapperInPlace(AliveCounterWrapperInPlace&&);
		~AliveCounterWrapperInPlace();

		void Test(int expectedValue);

		AliveCounterWrapperInPlace& operator=(const AliveCounterWrapperInPlace&);
		AliveCounterWrapperInPlace& operator=(AliveCounterWrapperInPlace&&);

	private:
		Nz::PrivateImpl<AliveCounter, 64> m_privateCounter;
};