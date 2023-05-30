#include <NazaraUtils/EnumMap.hpp>
#include <catch2/catch_test_macros.hpp>

enum class Test
{
	First,
	Second,
	Third,

	Max = Third
};

enum class NotSuitedEnum { A, B, };

SCENARIO("EnumMap", "[EnumMap]")
{
	// This should not compile
	// Nz::EnumMap<NotSuitedEnum, int> array;

	Nz::EnumMap<Test, int> array {
		1, 2, 3
	};

	CHECK(array[Test::First]  == 1);
	CHECK(array[Test::Second] == 2);
	CHECK(array[Test::Third]  == 3);

	std::size_t counter = 0;
	for (auto&& [enumKey, value] : array.iter_kv())
	{
		CHECK(Nz::UnderlyingCast(enumKey) == value - 1);
		counter++;
	}

	CHECK(counter == array.size());
}
