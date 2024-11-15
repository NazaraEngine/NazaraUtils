#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include "AliveCounterWrapper.hpp"

SCENARIO("PrivateImpl", "[PrivateImpl]")
{
	WHEN("When testing with dynamic allocation")
	{
		AliveCounterStruct counter;
		{
			AliveCounterWrapperDynAlloc counter1(&counter, 10);
			counter1.Test(10);

			CHECK(counter.aliveCount == 1);
			CHECK(counter.copyCount == 0);
			CHECK(counter.moveCount == 0);

			WHEN("Copy constructing")
			{
				AliveCounterWrapperDynAlloc counter2(counter1);
				counter2.Test(10);

				CHECK(counter.aliveCount == 2);
				CHECK(counter.copyCount == 1);
				CHECK(counter.moveCount == 0);
			}

			WHEN("Move constructing")
			{
				AliveCounterWrapperDynAlloc counter2(std::move(counter1));
				counter2.Test(10);

				CHECK(counter.aliveCount == 1);
				CHECK(counter.copyCount == 0);
				CHECK(counter.moveCount == 0);
			}

			WHEN("Copy assign")
			{
				AliveCounterWrapperDynAlloc counter2(nullptr, 0);
				counter2 = counter1;
				counter2.Test(10);

				CHECK(counter.aliveCount == 2);
				CHECK(counter.copyCount == 1);
				CHECK(counter.moveCount == 0);
			}

			WHEN("Move assign")
			{
				AliveCounterWrapperDynAlloc counter2(nullptr, 0);
				counter2 = std::move(counter1);
				counter2.Test(10);

				CHECK(counter.aliveCount == 1);
				CHECK(counter.copyCount == 0);
				CHECK(counter.moveCount == 0);
			}
		}

		CHECK(counter.aliveCount == 0);
	}

	WHEN("When testing with in-place allocation")
	{
		AliveCounterStruct counter;
		{
			AliveCounterWrapperInPlace counter1(&counter, 10);
			counter1.Test(10);

			CHECK(counter.aliveCount == 1);
			CHECK(counter.copyCount == 0);
			CHECK(counter.moveCount == 0);

			WHEN("Copy constructing")
			{
				AliveCounterWrapperInPlace counter2(counter1);
				counter2.Test(10);

				CHECK(counter.aliveCount == 2);
				CHECK(counter.copyCount == 1);
				CHECK(counter.moveCount == 0);
			}

			WHEN("Move constructing")
			{
				AliveCounterWrapperInPlace counter2(std::move(counter1));
				counter2.Test(10);

				CHECK(counter.aliveCount == 2);
				CHECK(counter.copyCount == 0);
				CHECK(counter.moveCount == 1);
			}

			WHEN("Copy assign")
			{
				AliveCounterWrapperInPlace counter2(nullptr, 0);
				counter2 = counter1;
				counter2.Test(10);

				CHECK(counter.aliveCount == 2);
				CHECK(counter.copyCount == 1);
				CHECK(counter.moveCount == 0);
			}

			WHEN("Move assign")
			{
				AliveCounterWrapperInPlace counter2(nullptr, 0);
				counter2 = std::move(counter1);
				counter2.Test(10);

				CHECK(counter.aliveCount == 2);
				CHECK(counter.copyCount == 0);
				CHECK(counter.moveCount == 1);
			}
		}

		CHECK(counter.aliveCount == 0);
	}
}
