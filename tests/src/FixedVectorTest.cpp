#include "AliveCounter.hpp"
#include <Nazara/Utils/FixedVector.hpp>
#include <Nazara/Utils/MovablePtr.hpp>
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <numeric>

// This is a quick way to check that checks are valid
#define USE_STD_VECTOR 0

SCENARIO("FixedVector", "[CORE][STACKVECTOR]")
{
	GIVEN("A FixedVector to contain multiple objects")
	{
		AliveCounter::Counter counter;
		{
			constexpr std::size_t capacity = 50;
#if USE_STD_VECTOR
			std::vector<AliveCounter> vector;
			vector.reserve(capacity);
#else
			Nz::FixedVector<AliveCounter, capacity> vector;
#endif

			WHEN("At construction, the vector is empty but has capacity")
			{
				CHECK(vector.capacity() == capacity);
				CHECK(vector.empty());
				CHECK(vector.size() == 0);
#if !USE_STD_VECTOR
				CHECK(vector.max_size() == capacity);
#endif
			}

			WHEN("Resizing it changes its size and create/destroy elements")
			{
				vector.resize(vector.capacity());
				CHECK(vector.size() == vector.capacity());
				CHECK(counter.aliveCount == 0);
				vector.resize(0);
				CHECK(vector.empty());
				CHECK(vector.size() == 0);
				CHECK(counter.aliveCount == 0);
			}

			WHEN("Resizing it allocates elements")
			{
				vector.resize(vector.capacity(), AliveCounter(&counter, 0));
				CHECK(vector.size() == vector.capacity());
				CHECK(counter.aliveCount == capacity);
				CHECK(counter.copyCount == vector.size());
				vector.resize(0);
				CHECK(vector.empty());
				CHECK(vector.size() == 0);
				CHECK(counter.aliveCount == 0);
			}

			WHEN("We can construct it with elements")
			{
				decltype(vector) vec(capacity, AliveCounter(&counter, 42));
				CHECK(vec.size() == capacity);
				CHECK(vec.capacity() == capacity);
				CHECK(counter.aliveCount == capacity);
				CHECK(counter.copyCount == capacity);
				CHECK(std::all_of(vec.begin(), vec.end(), [](const AliveCounter& counter) { return counter == 42; }));
			}

			WHEN("Emplacing five elements, vector size increase accordingly")
			{
				for (std::size_t i = 0; i < 5; ++i)
				{
#if USE_STD_VECTOR
					vector.emplace_back(&counter, int(i));
#else
					CHECK(vector.emplace_back(&counter, int(i)) == int(i));
#endif
				}

				CHECK(!vector.empty());
				CHECK(vector.size() == 5);

				std::array<int, 5> expectedValues = { 0, 1, 2, 3, 4 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}

			WHEN("Pushing three elements, vector size increase accordingly")
			{
				for (std::size_t i = 0; i < 3; ++i)
				{
					AliveCounter val(&counter, int(i));
#if USE_STD_VECTOR
					vector.push_back(val);
#else
					CHECK(vector.push_back(val) == val);
#endif
				}

				CHECK(!vector.empty());
				CHECK(vector.size() == 3);
				{
					std::array<int, 3> expectedValues = { 0, 1, 2 };
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
				}

				THEN("We resize to five")
				{
					AliveCounter::Counter counter2;
					vector.resize(5, AliveCounter(&counter2, 0));

					CHECK(!vector.empty());
					CHECK(vector.size() == 5);
					CHECK(counter2.aliveCount == 2);

					std::array<int, 5> expectedValues = { 0, 1, 2, 0, 0 };
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));

					AND_THEN("We resize it back to zero")
					{
						vector.resize(0);

						CHECK(vector.empty());
						CHECK(vector.size() == 0);
					}
					AND_THEN("We resize it back to zero by passing a reference")
					{
						AliveCounter::Counter counter3;
						vector.resize(0, AliveCounter(&counter3, 0));

						CHECK(vector.empty());
						CHECK(vector.size() == 0);
						CHECK(counter3.aliveCount == 0);
					}
					AND_THEN("We clear it")
					{
						vector.clear();

						CHECK(vector.empty());
						CHECK(vector.size() == 0);
						CHECK(counter.aliveCount == 0);
					}
				}
			}

			WHEN("We generate its content will iota")
			{
				vector.resize(10);
				for (std::size_t i = 0; i < vector.size(); ++i)
					vector[i] = AliveCounter(&counter, -5 + int(i));

				CHECK(counter.moveCount == 10);
				counter.moveCount = 0; // reset move count to make the following checks easier

				WHEN("We iterate on it")
				{
					std::array<int, 10> expectedValues = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4 };

					auto expected = expectedValues.begin();
					for (auto it = vector.begin(); it != vector.end(); ++it)
						CHECK(*it == *expected++);

					CHECK(expected == expectedValues.end());

					expected = expectedValues.begin();
					for (auto it = std::as_const(vector).begin(); it != std::as_const(vector).end(); ++it)
						CHECK(*it == *expected++);

					CHECK(expected == expectedValues.end());

					expected = expectedValues.begin();
					for (auto it = vector.cbegin(); it != vector.cend(); ++it)
						CHECK(*it == *expected++);

					CHECK(expected == expectedValues.end());
				}

				WHEN("We iterate on it in reverse")
				{
					std::array<int, 10> expectedValues = { 4, 3, 2, 1, 0, -1, -2, -3, -4, -5 };

					auto expected = expectedValues.begin();
					for (auto rit = vector.rbegin(); rit != vector.rend(); ++rit)
						CHECK(*rit == *expected++);

					CHECK(expected == expectedValues.end());

					expected = expectedValues.begin();
					for (auto rit = std::as_const(vector).rbegin(); rit != std::as_const(vector).rend(); ++rit)
						CHECK(*rit == *expected++);

					CHECK(expected == expectedValues.end());

					expected = expectedValues.begin();
					for (auto it = vector.crbegin(); it != vector.crend(); ++it)
						CHECK(*it == *expected++);

					CHECK(expected == expectedValues.end());
				}

				WHEN("We copy construct the vector")
				{
					decltype(vector) vec2(vector);
					CHECK(vec2.size() == vector.size());
					CHECK(vec2.capacity() == vector.capacity());
					CHECK(counter.aliveCount == vector.size() * 2);
					CHECK(counter.copyCount == vector.size());
					CHECK(std::equal(vector.begin(), vector.end(), vec2.begin(), vec2.end()));
				}

				WHEN("We move construct the vector")
				{
					decltype(vector) vec2(std::move(vector));
					CHECK(counter.moveCount == vec2.size());
					vector.clear();
					CHECK(counter.aliveCount == vec2.size());
					std::array<int, 10> expectedValues = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4 };
					CHECK(std::equal(vec2.begin(), vec2.end(), expectedValues.begin(), expectedValues.end()));
				}

				WHEN("We copy assign the vector")
				{
					AliveCounter::Counter counter2;
					decltype(vector) vec2(capacity, AliveCounter(&counter2, 42));
					CHECK(counter2.aliveCount == capacity);

					vec2 = vector;
					CHECK(counter.aliveCount == vector.size() * 2);
					CHECK(counter.copyCount == vector.size());
					CHECK(counter2.aliveCount == 0);
					CHECK(std::equal(vector.begin(), vector.end(), vec2.begin(), vec2.end()));

					vec2.clear();
					CHECK(counter.aliveCount == vector.size());
				}

				WHEN("We move assign the vector")
				{
					AliveCounter::Counter counter2;
					decltype(vector) vec2(capacity, AliveCounter(&counter2, 42));
					CHECK(counter2.aliveCount == capacity);

					vec2 = std::move(vector);
					CHECK(counter2.aliveCount == 0);
					CHECK(counter.moveCount == vec2.size());
					vector.clear();
					CHECK(counter.aliveCount == vec2.size());
					std::array<int, 10> expectedValues = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4 };
					CHECK(std::equal(vec2.begin(), vec2.end(), expectedValues.begin(), expectedValues.end()));
				}

				WHEN("We pop back some elements")
				{
					for (std::size_t i = 0; i < 5; ++i)
						vector.pop_back();

					std::array<int, 5> expectedValues = { -5, -4, -3, -2, -1 };
					CHECK(vector.size() == expectedValues.size());
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
				}
				WHEN("We erase elements at the beginning")
				{
					vector.erase(vector.begin());
					vector.erase(vector.begin());

					std::array<int, 8> expectedValues = { -3, -2, -1, 0, 1, 2, 3, 4 };
					CHECK(vector.size() == expectedValues.size());
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
				}
				WHEN("We erase elements in the middle")
				{
					vector.erase(vector.begin() + 2);
					vector.erase(vector.begin() + 2);
					vector.erase(vector.begin() + 6);

					std::array<int, 7> expectedValues = { -5, -4, -1, 0, 1, 2, 4 };
					CHECK(vector.size() == expectedValues.size());
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
				}
				WHEN("We erase elements at the end")
				{
					vector.erase(vector.end() - 1);
					vector.erase(vector.end() - 1);

					std::array<int, 8> expectedValues = { -5, -4, -3, -2, -1, 0, 1, 2 };
					CHECK(vector.size() == expectedValues.size());
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
				}
				WHEN("We erase a range")
				{
					vector.erase(vector.begin() + 2, vector.end() - 3);

					std::array<int, 5> expectedValues = { -5, -4, 2, 3, 4 };
					CHECK(vector.size() == expectedValues.size());
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
				}
				WHEN("We erase everything")
				{
					vector.erase(vector.begin(), vector.end());

					CHECK(vector.empty());
				}
			}

			WHEN("We generate its content using emplace")
			{
				for (std::size_t i = 0; i < 5; ++i)
					CHECK(*vector.emplace(vector.end(), &counter, int(i)) == int(i));

				CHECK(!vector.empty());
				CHECK(vector.size() == 5);

				std::array<int, 5> expectedValues = { 0, 1, 2, 3, 4 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}

			WHEN("We generate its content using emplace, in reverse order")
			{
				for (std::size_t i = 0; i < 5; ++i)
					CHECK(*vector.emplace(vector.begin(), &counter, int(i)) == int(i));

				CHECK(!vector.empty());
				CHECK(vector.size() == 5);

				std::array<int, 5> expectedValues = { 4, 3, 2, 1, 0 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}

			WHEN("We generate its content using emplace, at the middle")
			{
				for (std::size_t i = 0; i < 10; ++i)
					CHECK(*vector.emplace(vector.begin() + i / 2, &counter, int(i)) == int(i));

				CHECK(!vector.empty());
				CHECK(vector.size() == 10);

				std::array<int, 10> expectedValues = { 1, 3, 5, 7, 9, 8, 6, 4, 2, 0 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}

			WHEN("We generate its content using insert")
			{
				for (std::size_t i = 0; i < 5; ++i)
					CHECK(*vector.insert(vector.end(), AliveCounter(&counter, int(i))) == int(i));

				CHECK(!vector.empty());
				CHECK(vector.size() == 5);

				std::array<int, 5> expectedValues = { 0, 1, 2, 3, 4 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}

			WHEN("We generate its content using insert, in reverse order")
			{
				for (std::size_t i = 0; i < 5; ++i)
					CHECK(*vector.insert(vector.begin(), AliveCounter(&counter, int(i))) == int(i));

				CHECK(!vector.empty());
				CHECK(vector.size() == 5);

				std::array<int, 5> expectedValues = { 4, 3, 2, 1, 0 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}

			WHEN("We generate its content using insert, at the middle")
			{
				for (std::size_t i = 0; i < 10; ++i)
					CHECK(*vector.insert(vector.begin() + i / 2, AliveCounter(&counter, int(i))) == int(i));

				CHECK(!vector.empty());
				CHECK(vector.size() == 10);

				std::array<int, 10> expectedValues = { 1, 3, 5, 7, 9, 8, 6, 4, 2, 0 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}
		}

		CHECK(counter.aliveCount == 0);
	}
}
