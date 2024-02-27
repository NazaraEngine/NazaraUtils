#include <NazaraUtils/Flags.hpp>
#include <catch2/catch_test_macros.hpp>
#include <memory>

enum class Test
{
	A,
	B,
	C
};

using TestFlags = Nz::Flags<Test>;

template<>
struct Nz::EnumAsFlags<Test>
{
	static constexpr Test max = Test::C;
};

namespace Foo
{
	enum class Test2
	{
		A = 1,
		B = 2,
		C = 4,

		Count
	};

	constexpr bool EnableEnumAsNzFlags(Test2) { return true; }
	constexpr bool EnableAutoFlagForNzFlags(Test2) { return false; }

	using Test2Flags = Nz::Flags<Test2>;
}

template<typename E>
void TestIteration(Nz::Flags<E> flags, std::initializer_list<E> expected)
{
	std::size_t count = 0;
	auto it = expected.begin();

	for (E v : flags)
	{
		CHECK(v == *it++);
		count++;
	}

	CHECK(count == expected.size());
}

SCENARIO("Flags", "[Flags]")
{
	WHEN("We test flags")
	{
		TestFlags flags = Test::A | Test::C;
		CHECK(static_cast<Nz::UInt8>(flags) == (1 | 4));

		CHECK(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK(flags.Test(Test::C));

		TestIteration(flags, { Test::A, Test::C });
	}

	WHEN("We clear some flags")
	{
		TestFlags flags = Test::A | Test::C;
		flags.Clear(Test::B | Test::C);
		CHECK(static_cast<Nz::UInt16>(flags) == 1);

		CHECK(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK_FALSE(flags.Test(Test::C));

		TestIteration(flags, { Test::A });
	}

	WHEN("We clear all flags")
	{
		TestFlags flags = Test::A | Test::C;
		flags.Clear();
		CHECK(static_cast<Nz::UInt32>(flags) == 0);

		CHECK_FALSE(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK_FALSE(flags.Test(Test::C));

		TestIteration(flags, { });
	}

	WHEN("We set flags")
	{
		Foo::Test2Flags flags = Foo::Test2::A | Foo::Test2::C;
		flags.Set(Foo::Test2::B | Foo::Test2::C);
		CHECK(static_cast<Nz::UInt64>(flags) == (1 | 2 | 4));

		CHECK(flags.Test(Foo::Test2::A));
		CHECK(flags.Test(Foo::Test2::B));
		CHECK(flags.Test(Foo::Test2::C));

		TestIteration(flags, { Foo::Test2::A, Foo::Test2::B, Foo::Test2::C });
	}
}
