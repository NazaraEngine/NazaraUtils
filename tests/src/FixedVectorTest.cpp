#include "AliveCounter.hpp"
#include <NazaraUtils/FixedVector.hpp>
#include <NazaraUtils/MovablePtr.hpp>
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <numeric>

template<typename T>
struct ReferenceVector : public std::vector<T>
{
	using std::vector<T>::vector;

	static constexpr bool HasFallback = true;
	static constexpr std::size_t FixedCapacity = 0;
};

template<typename T>
void PerformVectorTest(const char* title)
{
	GIVEN(title)
	{
		AliveCounterStruct counter;
		{
			constexpr std::size_t elementCount = 50;
			T vector;

			WHEN("At construction, the vector is empty but has capacity")
			{
				CHECK(vector.capacity() == T::FixedCapacity);
				CHECK(vector.empty());
				CHECK(vector.size() == 0);
				if constexpr (!T::HasFallback)
					CHECK(vector.max_size() == T::FixedCapacity);
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
				vector.resize(elementCount, AliveCounter(&counter, 0));
				CHECK(vector.size() == elementCount);
				CHECK(counter.aliveCount == elementCount);
				CHECK(counter.copyCount >= vector.size()); // >= because libstd++ std::vector performs more copies than expected
				vector.resize(0);
				CHECK(vector.empty());
				CHECK(vector.size() == 0);
				CHECK(counter.aliveCount == 0);
			}

			WHEN("We reserve size before adding elements, no reallocation must happen")
			{
				CHECK(counter.moveCount == 0);
				vector.reserve(elementCount);
				for (std::size_t i = 0; i < elementCount; ++i)
					vector.emplace_back(&counter, int(i));

				CHECK(counter.moveCount == 0);
			}

			WHEN("We can construct it with elements")
			{
				decltype(vector) vec(elementCount, AliveCounter(&counter, 42));
				CHECK(vec.size() == elementCount);
				CHECK(vec.capacity() >= elementCount);
				CHECK(counter.aliveCount == elementCount);
				CHECK(counter.copyCount >= elementCount); // >= because libstd++ std::vector performs more copies than expected
				CHECK(std::all_of(vec.begin(), vec.end(), [](const AliveCounter& counter) { return counter == 42; }));
			}

			WHEN("Emplacing five elements, vector size increase accordingly")
			{
				for (std::size_t i = 0; i < 5; ++i)
					CHECK(vector.emplace_back(&counter, int(i)) == int(i));

				CHECK(!vector.empty());
				CHECK(vector.size() == 5);

				std::array<int, 5> expectedValues = { 0, 1, 2, 3, 4 };
				CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
			}

			WHEN("Pushing three elements, vector size increase accordingly")
			{
				CHECK(counter.aliveCount == 0);
				for (std::size_t i = 0; i < 3; ++i)
				{
					AliveCounter val(&counter, int(i));
					vector.push_back(val);
					CHECK(vector.size() == i + 1);

					CHECK(vector.back() == val);
				}
				CHECK(counter.aliveCount == 3);

				CHECK(!vector.empty());
				CHECK(vector.size() == 3);
				{
					std::array<int, 3> expectedValues = { 0, 1, 2 };
					CHECK(std::equal(vector.begin(), vector.end(), expectedValues.begin(), expectedValues.end()));
				}

				THEN("We resize to five")
				{
					AliveCounterStruct counter2;
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
						AliveCounterStruct counter3;
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
					if (vec2.size() < T::FixedCapacity)
						CHECK(vec2.capacity() == vector.capacity());
					CHECK(counter.aliveCount == vector.size() * 2);
					CHECK(counter.copyCount == vector.size());
					CHECK(std::equal(vector.begin(), vector.end(), vec2.begin(), vec2.end()));
				}

				WHEN("We move construct the vector")
				{
					decltype(vector) vec2(std::move(vector));
					if (vec2.size() < T::FixedCapacity)
						CHECK(counter.moveCount == vec2.size());
					vector.clear();
					CHECK(counter.aliveCount == vec2.size());
					std::array<int, 10> expectedValues = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4 };
					CHECK(std::equal(vec2.begin(), vec2.end(), expectedValues.begin(), expectedValues.end()));
				}

				WHEN("We copy assign the vector")
				{
					AliveCounterStruct counter2;
					decltype(vector) vec2(elementCount, AliveCounter(&counter2, 42));
					CHECK(counter2.aliveCount == elementCount);

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
					AliveCounterStruct counter2;
					decltype(vector) vec2(elementCount, AliveCounter(&counter2, 42));
					CHECK(counter2.aliveCount == elementCount);

					vec2 = std::move(vector);
					CHECK(counter2.aliveCount == 0);
					if (vec2.size() < T::FixedCapacity)
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

SCENARIO("FixedVector", "[CORE][STACKVECTOR]")
{
	GIVEN("A FixedVector to contain multiple objects")
	{
		PerformVectorTest<ReferenceVector<AliveCounter>>("std::vector (reference)");
		PerformVectorTest<Nz::FixedVector<AliveCounter, 50>>("Nz::FixedVector with no fallback");
		PerformVectorTest<Nz::HybridVector<AliveCounter, 2>>("Nz::HybridVector with a capacity of 2");
		PerformVectorTest<Nz::HybridVector<AliveCounter, 5>>("Nz::HybridVector with a capacity of 5");
		PerformVectorTest<Nz::HybridVector<AliveCounter, 100>>("Nz::HybridVector with a capacity of 100");
	}

	GIVEN("A FixedVector to contain objects without a default constructor")
	{
		std::size_t counter = 0;
		struct NoDefaultConstructor
		{
			NoDefaultConstructor(std::size_t& counter) :
			m_counter(counter)
			{
				m_counter++;
			}

			NoDefaultConstructor(const NoDefaultConstructor& obj) :
			m_counter(obj.m_counter)
			{
				m_counter++;
			}

			NoDefaultConstructor(NoDefaultConstructor&&) = delete;

			~NoDefaultConstructor()
			{
				m_counter--;
			}

			NoDefaultConstructor& operator=(const NoDefaultConstructor&) = delete;
			NoDefaultConstructor& operator=(NoDefaultConstructor&&) = delete;

			std::size_t& m_counter;
		};

		WHEN("Emplacing objects")
		{
			Nz::FixedVector<NoDefaultConstructor, 3> vec;
			vec.emplace_back(counter);
			vec.emplace_back(counter);
			vec.emplace_back(counter);

			CHECK(counter == 3);

			WHEN("Clearing vec")
			{
				CHECK(counter == 3);
				vec.clear();
				CHECK(counter == 0);
			}
		}

		WHEN("Constructing with objects")
		{
			Nz::FixedVector<NoDefaultConstructor, 3> vec{ counter, counter, counter };
			CHECK(counter == 3);

			WHEN("Clearing vec")
			{
				CHECK(counter == 3);
				vec.clear();
				CHECK(counter == 0);
			}
		}


		CHECK(counter == 0);
	}
}
