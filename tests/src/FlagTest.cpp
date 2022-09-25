#include <Nazara/Utils/Flags.hpp>
#include <catch2/catch_test_macros.hpp>

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

SCENARIO("Flags", "[Flags]")
{
	TestFlags flags = Test::A | Test::C;

	WHEN("We test flags")
	{
		CHECK(static_cast<Nz::UInt8>(flags) == (1 | 4));

		CHECK(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK(flags.Test(Test::C));
	}

	WHEN("We clear some flags")
	{
		flags.Clear(Test::B | Test::C);
		CHECK(static_cast<Nz::UInt16>(flags) == 1);

		CHECK(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK_FALSE(flags.Test(Test::C));
	}

	WHEN("We clear all flags")
	{
		flags.Clear();
		CHECK(static_cast<Nz::UInt32>(flags) == 0);

		CHECK_FALSE(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK_FALSE(flags.Test(Test::C));
	}

	WHEN("We set flags")
	{
		flags.Set(Test::B | Test::C);
		CHECK(static_cast<Nz::UInt64>(flags) == (1 | 2 | 4));

		CHECK(flags.Test(Test::A));
		CHECK(flags.Test(Test::B));
		CHECK(flags.Test(Test::C));
	}
}
