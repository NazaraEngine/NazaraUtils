#include <Nazara/Utils/Algorithm.hpp>
#include <catch2/catch_test_macros.hpp>
#include <map>

SCENARIO("Algorithm", "[Algorithm]")
{
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
