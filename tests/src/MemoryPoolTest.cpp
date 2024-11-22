#include <NazaraUtils/MemoryPool.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
	std::size_t allocationCount = 0;

	template<typename T>
	struct AllocatorTest : T
	{
		template<typename... Args>
		AllocatorTest(Args&&... args) :
		T(std::forward<Args>(args)...)
		{
			allocationCount++;
		}

		AllocatorTest(const AllocatorTest&) = delete;
		AllocatorTest(AllocatorTest&&) = delete;

		~AllocatorTest()
		{
			assert(allocationCount > 0);
			allocationCount--;
		}
	};

	struct Vector2
	{
		Vector2(int X, int Y) :
		x(X),
		y(Y)
		{
		}

		bool operator==(const Vector2& vec) const
		{
			return x == vec.x && y == vec.y;
		}

		int x;
		int y;
	};
}

SCENARIO("MemoryPool", "[CORE][MEMORYPOOL]")
{
	GIVEN("A MemoryPool to contain one Vector2")
	{
		using T = AllocatorTest<Vector2>;

		allocationCount = 0;

		Nz::MemoryPool<T> memoryPool(2);
		CHECK(memoryPool.GetAllocatedEntryCount() == 0);
		CHECK(memoryPool.GetBlockCount() == 1);
		CHECK(memoryPool.GetBlockSize() == 2);
		CHECK(memoryPool.GetFreeEntryCount() == 2);
		CHECK(allocationCount == 0);

		WHEN("We construct a Vector2")
		{
			std::size_t index;
			T* vector2 = memoryPool.Allocate(index, 1, 2);
			CHECK(allocationCount == 1);
			CHECK(memoryPool.GetAllocatedEntryCount() == 1);
			CHECK(memoryPool.GetFreeEntryCount() == 1);
			CHECK(memoryPool.RetrieveEntryIndex(vector2) == index);

			THEN("Memory is available")
			{
				vector2->x = 3;
				REQUIRE(*vector2 == Vector2{ 3, 2 });
			}

			memoryPool.Free(index);
			CHECK(allocationCount == 0);
			CHECK(memoryPool.GetAllocatedEntryCount() == 0);
			CHECK(memoryPool.GetFreeEntryCount() == 2);
		}

		WHEN("We construct three vectors")
		{
			CHECK(memoryPool.GetAllocatedEntryCount() == 0);
			CHECK(memoryPool.GetFreeEntryCount() == 2);

			std::size_t index1, index2, index3;
			T* vector1 = memoryPool.Allocate(index1, 1, 2);
			CHECK(allocationCount == 1);
			CHECK(memoryPool.GetAllocatedEntryCount() == 1);
			CHECK(memoryPool.GetBlockCount() == 1);
			CHECK(memoryPool.GetFreeEntryCount() == 1);
			T* vector2 = memoryPool.Allocate(index2, 3, 4);
			CHECK(allocationCount == 2);
			CHECK(memoryPool.GetAllocatedEntryCount() == 2);
			CHECK(memoryPool.GetBlockCount() == 1);
			CHECK(memoryPool.GetFreeEntryCount() == 0);
			T* vector3 = memoryPool.Allocate(index3, 5, 6);
			CHECK(allocationCount == 3);
			CHECK(memoryPool.GetAllocatedEntryCount() == 3);
			CHECK(memoryPool.GetBlockCount() == 2);
			CHECK(memoryPool.GetFreeEntryCount() == 1); //< a new block has been allocated

			CHECK(memoryPool.RetrieveEntryIndex(vector1) == index1);
			CHECK(memoryPool.RetrieveEntryIndex(vector2) == index2);
			CHECK(memoryPool.RetrieveEntryIndex(vector3) == index3);

			THEN("Memory is available")
			{
				vector1->x = 3;
				vector2->y = 5;
				CHECK(*vector1 == Vector2{ 3, 2 });
				CHECK(*vector2 == Vector2{ 3, 5 });
				CHECK(*vector3 == Vector2{ 5, 6 });

				AND_THEN("We iterate on the memory pool")
				{
					std::size_t count = 0;
					int sumX = 0;
					int sumY = 0;
					for (T& vec : memoryPool)
					{
						count++;
						sumX += vec.x;
						sumY += vec.y;
					}

					CHECK(count == 3);
					CHECK(sumX == 11);
					CHECK(sumY == 13);
				}
				AND_THEN("We iterate on the memory pool (const)")
				{
					std::size_t count = 0;
					for (const T& vec : std::as_const(memoryPool))
					{
						NazaraUnused(vec);
						count++;
					}

					CHECK(count == 3);
				}
			}

			memoryPool.Reset();
			CHECK(allocationCount == 0);
			CHECK(memoryPool.GetAllocatedEntryCount() == 0);
			CHECK(memoryPool.GetBlockCount() == 2);
			CHECK(memoryPool.GetFreeEntryCount() == 4);

			bool failure = false;
			for (T& vec : memoryPool)
			{
				NazaraUnused(vec);
				failure = true;
			}

			CHECK_FALSE(failure);

			memoryPool.Clear();
			CHECK(allocationCount == 0);
			CHECK(memoryPool.GetAllocatedEntryCount() == 0);
			CHECK(memoryPool.GetBlockCount() == 0);
			CHECK(memoryPool.GetFreeEntryCount() == 0);
		}
	}
}
