#include <NazaraUtils/PathUtils.hpp>
#include <catch2/catch_test_macros.hpp>

SCENARIO("PathUtils", "[PathUtils]")
{
	SECTION("Testing path functions")
	{
		CHECK(Nz::PathToString(Nz::Utf8Path("test")) == "test");
	}
}
