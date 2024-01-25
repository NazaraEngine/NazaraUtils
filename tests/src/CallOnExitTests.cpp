#include <NazaraUtils/CallOnExit.hpp>
#include <catch2/catch_test_macros.hpp>

SCENARIO("CallOnExit", "[CallOnExit]")
{
	WHEN("Checking if CallOnExit is called at end of scope")
	{
		bool called = false;
		{
			Nz::CallOnExit callOnExit([&]{ called = true; });
			CHECK_FALSE(called);
		}
		CHECK(called);
	}

	WHEN("Checking if CallOnExit is called when reset")
	{
		bool called = false;
		{
			Nz::CallOnExit callOnExit([&] { called = true; });
			CHECK_FALSE(called);
			callOnExit.Reset();
		}
		CHECK_FALSE(called);
	}

	WHEN("Checking that CallOnExit is still called only once when CallAndReset is used")
	{
		unsigned int callCount = 0;
		{
			Nz::CallOnExit callOnExit([&] { callCount++; });
			CHECK(callCount == 0);
			callOnExit.CallAndReset();
		}
		CHECK(callCount == 1);
	}

	WHEN("Testing NAZARA_DEFER")
	{
		bool called = false;
		{
			NAZARA_DEFER({ called = true; });
			CHECK_FALSE(called);
		}
		CHECK(called);
	}
}
