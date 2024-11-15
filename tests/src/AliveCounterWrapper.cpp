#include "AliveCounterWrapper.hpp"
#include "AliveCounter.hpp"
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

AliveCounterWrapperDynAlloc::AliveCounterWrapperDynAlloc(AliveCounterStruct* counter, int value) :
m_privateCounter(counter, value)
{
}

AliveCounterWrapperDynAlloc::AliveCounterWrapperDynAlloc(const AliveCounterWrapperDynAlloc&) = default;
AliveCounterWrapperDynAlloc::AliveCounterWrapperDynAlloc(AliveCounterWrapperDynAlloc&&) = default;
AliveCounterWrapperDynAlloc::~AliveCounterWrapperDynAlloc() = default;

void AliveCounterWrapperDynAlloc::Test(int expectedValue)
{
	CHECK(m_privateCounter.Get()->GetValue() == expectedValue);
	CHECK(std::as_const(m_privateCounter).Get()->GetValue() == expectedValue);
	CHECK((*m_privateCounter).GetValue() == expectedValue);
	CHECK(m_privateCounter->GetValue() == expectedValue);
	CHECK(static_cast<AliveCounter*>(m_privateCounter)->GetValue() == expectedValue);
	CHECK(std::as_const(*m_privateCounter).GetValue() == expectedValue);
	CHECK(std::as_const(m_privateCounter)->GetValue() == expectedValue);
	CHECK(static_cast<const AliveCounter*>(std::as_const(m_privateCounter))->GetValue() == expectedValue);
}

AliveCounterWrapperDynAlloc& AliveCounterWrapperDynAlloc::operator=(const AliveCounterWrapperDynAlloc&) = default;
AliveCounterWrapperDynAlloc& AliveCounterWrapperDynAlloc::operator=(AliveCounterWrapperDynAlloc&&) = default;

AliveCounterWrapperInPlace::AliveCounterWrapperInPlace(AliveCounterStruct* counter, int value) :
m_privateCounter(counter, value)
{
}

AliveCounterWrapperInPlace::AliveCounterWrapperInPlace(const AliveCounterWrapperInPlace&) = default;
AliveCounterWrapperInPlace::AliveCounterWrapperInPlace(AliveCounterWrapperInPlace&&) = default;
AliveCounterWrapperInPlace::~AliveCounterWrapperInPlace() = default;

void AliveCounterWrapperInPlace::Test(int expectedValue)
{
	CHECK(m_privateCounter.Get()->GetValue() == expectedValue);
	CHECK(std::as_const(m_privateCounter).Get()->GetValue() == expectedValue);
	CHECK((*m_privateCounter).GetValue() == expectedValue);
	CHECK(m_privateCounter->GetValue() == expectedValue);
	CHECK(static_cast<AliveCounter*>(m_privateCounter)->GetValue() == expectedValue);
	CHECK(std::as_const(*m_privateCounter).GetValue() == expectedValue);
	CHECK(std::as_const(m_privateCounter)->GetValue() == expectedValue);
	CHECK(static_cast<const AliveCounter*>(std::as_const(m_privateCounter))->GetValue() == expectedValue);
}

AliveCounterWrapperInPlace& AliveCounterWrapperInPlace::operator=(const AliveCounterWrapperInPlace&) = default;
AliveCounterWrapperInPlace& AliveCounterWrapperInPlace::operator=(AliveCounterWrapperInPlace&&) = default;