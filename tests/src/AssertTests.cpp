#include <NazaraUtils/Assert.hpp>
#include <catch2/catch_test_macros.hpp>

SCENARIO("Assert", "[Assert]")
{
	NazaraAssert(true);
	NazaraAssertMsg(true, "error message with value = %d", 42);
}
