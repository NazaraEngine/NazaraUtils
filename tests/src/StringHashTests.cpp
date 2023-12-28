#include <NazaraUtils/Algorithm.hpp>
#include <NazaraUtils/StringHash.hpp>
#include <catch2/catch_test_macros.hpp>
#include <unordered_map>

#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20)

TEST_CASE("StringHash", "[StringHash]")
{
	SECTION("Test std::unordered_map with multiple key types")
	{
		using namespace std::literals;

		std::unordered_map<std::string, unsigned int, Nz::StringHash<>, std::equal_to<>> map;
		map["test1"] = 1;
		map["test2"] = 2;
		map["test3"] = 3;

		CHECK(Nz::Retrieve(map, "test1"sv) == 1);
		CHECK(Nz::Retrieve(map, "test2"s) == 2);
		CHECK(Nz::Retrieve(map, "test3") == 3);
	}
}

#endif
