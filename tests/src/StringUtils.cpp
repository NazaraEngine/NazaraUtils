#include <NazaraUtils/StringUtils.hpp>
#include <catch2/catch_test_macros.hpp>

SCENARIO("StringUtils", "[StringUtils]")
{
	SECTION("Testing UTF-8 functions")
	{
		using namespace std::literals;

		CHECK(Nz::FromUtf8String(u8"test") == "test");
		CHECK(Nz::FromUtf8String(u8"test"s) == "test");
		CHECK(Nz::FromUtf8String(u8"test"sv) == "test");
		CHECK(Nz::ToUtf8String("test") == u8"test");
		CHECK(Nz::ToUtf8String("test"s) == u8"test");
		CHECK(Nz::ToUtf8String("test"sv) == u8"test");
	}
}
