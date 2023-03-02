#pragma once

#include <cassert>
#include <cstddef>

class AliveCounter
{
	public:
		struct Counter
		{
			std::size_t aliveCount = 0;
			std::size_t copyCount = 0;
			std::size_t moveCount = 0;
		};

		AliveCounter() :
		m_counter(nullptr),
		m_value(0)
		{
		}

		AliveCounter(Counter* counter, int value) :
		m_counter(counter),
		m_value(value)
		{
			if (m_counter)
				m_counter->aliveCount++;
		}

		AliveCounter(const AliveCounter& counter) :
		m_counter(counter.m_counter),
		m_value(counter.m_value)
		{
			if (m_counter)
			{
				m_counter->aliveCount++;
				m_counter->copyCount++;
			}
		}

		AliveCounter(AliveCounter&& counter) :
		m_counter(counter.m_counter),
		m_value(counter.m_value)
		{
			if (m_counter)
			{
				m_counter->aliveCount++;
				m_counter->moveCount++;
			}
		}

		~AliveCounter()
		{
			if (m_counter)
			{
				assert(m_counter->aliveCount > 0);
				m_counter->aliveCount--;
			}
		}

		operator int() const
		{
			return m_value;
		}

		AliveCounter& operator=(const AliveCounter& counter)
		{
			if (m_counter)
			{
				assert(m_counter->aliveCount > 0);
				m_counter->aliveCount--;
			}

			m_counter = counter.m_counter;
			m_value = counter.m_value;

			if (m_counter)
			{
				m_counter->aliveCount++;
				m_counter->copyCount++;
			}

			return *this;
		}

		AliveCounter& operator=(AliveCounter&& counter)
		{
			if (this == &counter)
				return *this;

			if (m_counter)
			{
				assert(m_counter->aliveCount > 0);
				m_counter->aliveCount--;
			}

			m_counter = counter.m_counter;
			m_value = counter.m_value;

			if (m_counter)
			{
				m_counter->aliveCount++;
				m_counter->moveCount++;
			}

			return *this;
		}

	private:
		Counter* m_counter;
		int m_value;
};
