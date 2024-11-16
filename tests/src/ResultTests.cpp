#include <NazaraUtils/Algorithm.hpp>
#include <NazaraUtils/Result.hpp>
#include <CopyCounter.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

namespace
{
	Nz::Result<std::string, std::string> AlwaysSucceed(const char* value = "42")
	{
		return Nz::Ok(value);
	}

	Nz::Result<std::string, std::string> AlwaysFails(const char* err = "An error occurred")
	{
		return Nz::Err(err);
	}

	Nz::Result<std::string, std::string> Test1()
	{
		NAZARA_TRY(AlwaysFails("Err"));
		NAZARA_TRY_VALUE(std::string val, AlwaysSucceed("Ok"));

		return Nz::Ok(val);
	}

	Nz::Result<std::string, std::string> Test2()
	{
		NAZARA_TRY(AlwaysSucceed("Ok"));
		NAZARA_TRY_VALUE(std::string val, AlwaysFails("Err"));

		return Nz::Ok(val);
	}

	Nz::Result<std::string, std::string> Test3()
	{
		NAZARA_TRY(AlwaysSucceed("Ok1"));
		NAZARA_TRY_VALUE(std::string val, AlwaysSucceed("Ok2"));

		return Nz::Ok(val);
	}


	Nz::Result<CopyCounter, std::string> GetCopyCounter()
	{
		return CopyCounter{};
	}

	struct A {};
	struct B : A {};

	struct DefaultConstructor
	{
		DefaultConstructor()
		{
			v = 42;
		}

		int v = 0;
	};

	struct NoDefaultConstructor
	{
		NoDefaultConstructor(int value) : v(value) {}

		int v = 0;
	};
}

SCENARIO("Result", "[Result]")
{
	WHEN("Default creating a result")
	{
		Nz::Result<DefaultConstructor, NoDefaultConstructor> defaultConstructed;
		CHECK(defaultConstructed->v == 42);

		Nz::Result<NoDefaultConstructor, DefaultConstructor> noConstructor = Nz::Err(DefaultConstructor{});
		CHECK_THROWS(noConstructor->v == 42);
	}

	WHEN("Handling a success")
	{
		Nz::Result test = AlwaysSucceed();
		CHECK(test.IsOk());
		CHECK_FALSE(test.IsErr());
		CHECK(test.GetValue() == "42");
		CHECK(test.GetValueOr("94") == "42");
		CHECK(*test == "42");
		CHECK(test->size() == 2);

		CHECK_THROWS_WITH(test.GetError(), "Result is not an error");

		Nz::Result<std::string, bool> castedOk = Nz::Ok(test.GetValue());

		Nz::Result remappedResult = test.Map([](const std::string& value) { return std::stoi(value); });
		CHECK(remappedResult.GetValue() == 42);

		std::string originalValue;
		Nz::Result voidResult = std::move(test).Map([&](std::string&& value)
		{ 
			originalValue = std::move(value);
		});
		CHECK(originalValue == "42");

		Nz::Result remappedVoid = voidResult.Map([] { return CopyCounter{}; });
		CHECK(remappedVoid.IsOk());
		CHECK_FALSE(remappedVoid.IsErr());
		CHECK(remappedVoid.GetValue().GetCopyCount() == 0);

		voidResult = Nz::Ok();
	}

	WHEN("Handling a failure")
	{
		Nz::Result test = AlwaysFails();
		CHECK_FALSE(test.IsOk());
		CHECK(test.IsErr());
		CHECK(test.GetError() == "An error occurred");

		CHECK_THROWS_WITH(test.Expect("Unexpected failure"), "Unexpected failure");
		CHECK_THROWS_WITH(test.GetValue() == "Hello world", "Result is not a value");

		CHECK(test.GetValueOr("This is a value") == "This is a value");

		Nz::Result voidResult = std::move(test).Map([&](std::string&& /*value*/)
		{
			// Shouldn't be called
			FAIL();
		});
		CHECK_FALSE(voidResult.IsOk());
		CHECK(voidResult.IsErr());
		CHECK(voidResult.GetError() == "An error occurred");
		CHECK_THROWS_WITH(voidResult.Expect("Unexpected failure with void type"), "Unexpected failure with void type");

		Nz::Result remappedResult = voidResult.Map([] { return 42; });
		CHECK(remappedResult.IsErr());
		CHECK(remappedResult.GetError() == voidResult.GetError());

		Nz::Result<void, std::string> castedErr = Nz::Err(remappedResult.GetError());
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
			return std::move(counter);
		});

		CHECK(movedTest.GetValue().GetCopyCount() == 0);


		Nz::Result<void, CopyCounter> err = Nz::Err(CopyCounter{});
		CHECK(err.IsErr());
		CHECK_FALSE(err.IsOk());
		CHECK(err.GetError().GetCopyCount() == 0);
		CHECK(err.GetError().GetMoveCount() == 1);
		
		Nz::Result<int, CopyCounter> copiedErr = err.Map([]
		{
			FAIL();
			return -42;
		});
		CHECK(copiedErr.GetError().GetCopyCount() == 1);
		CHECK(copiedErr.GetError().GetMoveCount() == 1);

		Nz::Result<int, CopyCounter> movedErr = std::move(err).Map([]
		{
			FAIL();
			return -42;
		});
		CHECK(movedErr.GetError().GetCopyCount() == 0);
		CHECK(movedErr.GetError().GetMoveCount() == 2);
	}

	WHEN("Performing automatic casts")
	{
		Nz::Result<short, std::string> result = Nz::Ok(42);
		Nz::Result<int, std::string> result2 = result;
		CHECK(result2.GetValue() == result.GetValue());

		Nz::Result<B*, std::string> err = Nz::Err("Something went wrong");
		CHECK(err.GetError() == "Something went wrong");

		Nz::Result<A*, std::string_view> castedErr = err;
		CHECK(castedErr.GetError() == err.GetError());
	}

	WHEN("Using try macros")
	{
		Nz::Result result1 = Test1();
		CHECK(result1.IsErr());
		CHECK(result1.GetError() == "Err");

		Nz::Result result2 = Test2();
		CHECK(result2.IsErr());
		CHECK(result2.GetError() == "Err");

		Nz::Result result3 = Test3();
		CHECK(result3.IsOk());
		CHECK(result3.GetValue() == "Ok2");
	}
}
