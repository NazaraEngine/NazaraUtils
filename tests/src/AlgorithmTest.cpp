#include <NazaraUtils/Algorithm.hpp>
#include <catch2/catch_test_macros.hpp>
#include <map>

template<typename T>
void TestCountBits()
{
	T value = 0;
	CHECK(Nz::CountBits(value) == 0);
	CHECK(Nz::CountBits(std::numeric_limits<T>::max()) == Nz::BitCount<T>());
	CHECK(Nz::CountBits(std::numeric_limits<T>::max() / 2) == Nz::BitCount<T>() - 1);

	for (std::size_t i = 0; i < Nz::BitCount<T>(); ++i)
	{
		value = T(1) << i;
		CHECK(Nz::CountBits(value) == 1);
	}
}

template<typename T>
void TestFindFirstBit()
{
	T value = 0;
	CHECK(Nz::FindFirstBit(value) == 0);

	for (std::size_t i = 0; i < Nz::BitCount<T>(); ++i)
	{
		value = T(1) << i;
		CHECK(Nz::FindFirstBit(value) == i + 1);
	}
}

SCENARIO("Algorithm", "[Algorithm]")
{
	WHEN("Testing SafeCast")
	{
		// Catch2 offers no way to catch an assertion failure
		CHECK(Nz::SafeCast<Nz::UInt64>(3.0) == 3);
		CHECK(Nz::SafeCast<Nz::UInt64>(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
		CHECK(Nz::SafeCast<int>(Nz::UInt64(42)) == 42);
	}

	WHEN("Testing Retrieve")
	{
		using namespace std::literals;

		std::map<std::string, std::size_t, std::less<>> strMap = {
			{
				{ "Foo", 1 },
				{ "Bar", 2 },
				{ "Baz", 3 },
			}
		};

		CHECK(Nz::Retrieve(strMap, "Foo") == 1);
		CHECK(Nz::Retrieve(strMap, "Bar") == 2);
		CHECK(Nz::Retrieve(strMap, "Baz") == 3);

		CHECK(Nz::Retrieve(strMap, "Foo"sv) == 1);
		CHECK(Nz::Retrieve(strMap, "Bar"sv) == 2);
		CHECK(Nz::Retrieve(strMap, "Baz"sv) == 3);

		CHECK(Nz::Retrieve(strMap, "Foo"s) == 1);
		CHECK(Nz::Retrieve(strMap, "Bar"s) == 2);
		CHECK(Nz::Retrieve(strMap, "Baz"s) == 3);
	}

	WHEN("Testing CountBit")
	{
		TestCountBits<Nz::UInt8>();
		TestCountBits<Nz::UInt16>();
		TestCountBits<Nz::UInt32>();
		TestCountBits<Nz::UInt64>();
	}

	WHEN("Testing FindFirstBit")
	{
		TestFindFirstBit<Nz::UInt8>();
		TestFindFirstBit<Nz::UInt16>();
		TestFindFirstBit<Nz::UInt32>();
		TestFindFirstBit<Nz::UInt64>();
	}
}
