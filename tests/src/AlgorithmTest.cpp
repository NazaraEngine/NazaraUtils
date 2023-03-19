#include <NazaraUtils/Algorithm.hpp>
#include <catch2/catch_test_macros.hpp>
#include <map>

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
}
