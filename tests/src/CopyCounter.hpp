#pragma once

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
