#include <NazaraUtils/FunctionRef.hpp>
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
		if (!func)
			return -42;
		else
			return func();
	}

	int CallbackMethod(Nz::FunctionRef<int(Foo* foo)> func, Foo* foo)
	{
		CHECK(func);
		return func(foo);
	}

	std::size_t PassByRef(Nz::FunctionRef<std::size_t(const CopyCounter& counter)> func, const CopyCounter& counter)
	{
		CHECK(func);
		return func(counter);
	}

	std::size_t PassByValue(Nz::FunctionRef<std::size_t(CopyCounter counter)> func, CopyCounter counter)
	{
		CHECK(func);
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
	CHECK(Callback(nullptr) == -42);
	CHECK(Callback([] { return 0; }) == 0);
	CHECK(Callback([] { return 42; }) == 42);
	CHECK(Callback(FuncCall<47>) == 47);
	CHECK(Callback(&FuncCall<47>) == 47);

	Foo foo(42);
	CHECK(CallbackMethod(&Foo::Bar, &foo) == 42);

	CHECK(PassByValue([](CopyCounter counter) { return counter.GetCopyCount(); }, CopyCounter{}) == 0);
	CHECK(PassByRef([](const CopyCounter& counter) { return counter.GetCopyCount() + counter.GetMoveCount(); }, CopyCounter{}) == 0);

	Nz::FunctionRef<void(int)> nullRef(nullptr);
	CHECK_FALSE(nullRef);
}
