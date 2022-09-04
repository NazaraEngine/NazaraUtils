#include <Nazara/Utils/FunctionRef.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
	class CopyCounter
	{
		public:
			CopyCounter() :
			m_copyCount(0),
			m_moveCount(0)
			{
			}

			CopyCounter(const CopyCounter& counter) :
			m_copyCount(counter.m_copyCount + 1),
			m_moveCount(counter.m_moveCount)
			{
			}

			CopyCounter(CopyCounter&& counter) noexcept :
			m_copyCount(counter.m_copyCount),
			m_moveCount(counter.m_moveCount + 1)
			{
			}

			~CopyCounter() = default;

			CopyCounter& operator=(const CopyCounter& counter)
			{
				m_copyCount = counter.m_copyCount + 1;
				m_moveCount = counter.m_moveCount;

				return *this;
			}

			CopyCounter& operator=(CopyCounter&& counter) noexcept
			{
				m_copyCount = counter.m_copyCount;
				m_moveCount = counter.m_moveCount + 1;

				return *this;
			}

			std::size_t GetCopyCount() const
			{
				return m_copyCount;
			}

			std::size_t GetMoveCount() const
			{
				return m_moveCount;
			}

		private:
			std::size_t m_copyCount;
			std::size_t m_moveCount;
	};

	int Callback(Nz::FunctionRef<int()> func)
	{
		return func();
	}

	int PassByRef(Nz::FunctionRef<std::size_t(const CopyCounter& counter)> func, const CopyCounter& counter)
	{
		return func(counter);
	}

	int PassByValue(Nz::FunctionRef<std::size_t(CopyCounter counter)> func, CopyCounter counter)
	{
		return func(std::move(counter));
	}

	template<int N>
	int FuncCall()
	{
		return N;
	}
}

SCENARIO("FunctionRef", "[FunctionRef]")
{
	CHECK(Callback([] { return 0; }) == 0);
	CHECK(Callback([] { return 42; }) == 42);
	CHECK(Callback(FuncCall<47>) == 47);
	CHECK(Callback(&FuncCall<47>) == 47);

	CHECK(PassByValue([](CopyCounter counter) { return counter.GetCopyCount(); }, CopyCounter{}) == 0);
	CHECK(PassByRef([](const CopyCounter& counter) { return counter.GetCopyCount() + counter.GetMoveCount(); }, CopyCounter{}) == 0);
}
