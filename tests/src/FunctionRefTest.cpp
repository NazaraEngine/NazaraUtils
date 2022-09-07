#include <Nazara/Utils/FunctionRef.hpp>
#include <CopyCounter.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
	struct Foo
	{
		Foo(int value) :
		m_value(value)
		{
		}

		int Bar() { return m_value; }

		int m_value;
	};

	int Callback(Nz::FunctionRef<int()> func)
	{
		return func();
	}

	int CallbackMethod(Nz::FunctionRef<int(Foo* foo)> func, Foo* foo)
	{
		return func(foo);
	}

	std::size_t PassByRef(Nz::FunctionRef<std::size_t(const CopyCounter& counter)> func, const CopyCounter& counter)
	{
		return func(counter);
	}

	std::size_t PassByValue(Nz::FunctionRef<std::size_t(CopyCounter counter)> func, CopyCounter counter)
	{
		return func(std::move(counter));
	}

	template<int N>
	int FuncCall()
	{
		return N;
	}
}

SCENARIO("FunctionRef", "[FunctionRef]")
{
	CHECK(Callback([] { return 0; }) == 0);
	CHECK(Callback([] { return 42; }) == 42);
	CHECK(Callback(FuncCall<47>) == 47);
	CHECK(Callback(&FuncCall<47>) == 47);

	Foo foo(42);
	CHECK(CallbackMethod(&Foo::Bar, &foo) == 42);

	CHECK(PassByValue([](CopyCounter counter) { return counter.GetCopyCount(); }, CopyCounter{}) == 0);
	CHECK(PassByRef([](const CopyCounter& counter) { return counter.GetCopyCount() + counter.GetMoveCount(); }, CopyCounter{}) == 0);
}
