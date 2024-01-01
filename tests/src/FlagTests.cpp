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

void TestIteration(TestFlags flags, std::initializer_list<Test> expected)
{
	std::size_t count = 0;
	auto it = expected.begin();

	for (Test v : flags)
	{
		CHECK(v == *it++);
		count++;
	}

	CHECK(count == expected.size());
}

SCENARIO("Flags", "[Flags]")
{
	TestFlags flags = Test::A | Test::C;

	WHEN("We test flags")
	{
		CHECK(static_cast<Nz::UInt8>(flags) == (1 | 4));

		CHECK(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK(flags.Test(Test::C));

		TestIteration(flags, { Test::A, Test::C });
	}

	WHEN("We clear some flags")
	{
		flags.Clear(Test::B | Test::C);
		CHECK(static_cast<Nz::UInt16>(flags) == 1);

		CHECK(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK_FALSE(flags.Test(Test::C));

		TestIteration(flags, { Test::A });
	}

	WHEN("We clear all flags")
	{
		flags.Clear();
		CHECK(static_cast<Nz::UInt32>(flags) == 0);

		CHECK_FALSE(flags.Test(Test::A));
		CHECK_FALSE(flags.Test(Test::B));
		CHECK_FALSE(flags.Test(Test::C));

		TestIteration(flags, { });
	}

	WHEN("We set flags")
	{
		flags.Set(Test::B | Test::C);
		CHECK(static_cast<Nz::UInt64>(flags) == (1 | 2 | 4));

		CHECK(flags.Test(Test::A));
		CHECK(flags.Test(Test::B));
		CHECK(flags.Test(Test::C));

		TestIteration(flags, { Test::A, Test::B, Test::C });
	}
}
