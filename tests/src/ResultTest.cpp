#include <Nazara/Utils/Algorithm.hpp>
#include <Nazara/Utils/Result.hpp>
#include <CopyCounter.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

namespace
{
	Nz::Result<std::string, std::string> AlwaysSucceed()
	{
		return Nz::Ok<std::string>("42");
	}

	Nz::Result<std::string, std::string> AlwaysFails()
	{
		return Nz::Err<std::string>("An error occurred");
	}

	Nz::Result<CopyCounter, std::string> GetCopyCounter()
	{
		return CopyCounter{};
	}
}

SCENARIO("Result", "[Result]")
{
	WHEN("Handling a success")
	{
		Nz::Result test = AlwaysSucceed();
		CHECK(test.IsOk());
		CHECK(!test.IsErr());
		CHECK(test.GetValue() == "42");
		CHECK(test.GetValueOr("94") == "42");

		CHECK_THROWS_WITH(test.GetError(), "Result is not an error");

		Nz::Result remappedResult = test.Map([](const std::string& value) { return std::stoi(value); });
		CHECK(remappedResult.GetValue() == 42);

		std::string originalValue;
		Nz::Result remappedToVoid = std::move(test).Map([&](std::string&& value)
		{ 
			originalValue = std::move(value);
		});
		CHECK(originalValue == "42");
	}

	WHEN("Handling a failure")
	{
		Nz::Result test = AlwaysFails();
		CHECK(!test.IsOk());
		CHECK(test.IsErr());
		CHECK(test.GetError() == "An error occurred");

		CHECK_THROWS_WITH(test.GetValue() == "Hello world", "Result is not a value");

		CHECK(test.GetValueOr("This is a value") == "This is a value");

		Nz::Result remappedResult = std::move(test).Map([&](std::string&& /*value*/)
		{
			// Shouldn't be called
			FAIL();
		});
		CHECK(remappedResult.IsErr());
		CHECK(remappedResult.GetError() == test.GetError());
	}

	WHEN("Checking no avoidable copy occurs")
	{
		Nz::Result test = GetCopyCounter();
		CHECK(test.GetValue().GetCopyCount() == 0);
		CHECK(test.GetValue().GetMoveCount() == 1);
		CHECK(test.GetValueOr(CopyCounter{}).GetCopyCount() == 1); //< unavoidable copy (?)
		
		Nz::Result copiedTest = test.Map([](const CopyCounter& counter)
		{
			CHECK(counter.GetCopyCount() == 0);
			return counter;
		});

		CHECK(copiedTest.GetValue().GetCopyCount() == 1);
		
		Nz::Result movedTest = std::move(test).Map([](CopyCounter&& counter)
		{
			CHECK(counter.GetCopyCount() == 0);
			return counter;
		});

		CHECK(movedTest.GetValue().GetCopyCount() == 0);
	}
}
