// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <algorithm>
#include <cassert>
#include <new>
#include <utility>

namespace Nz
{
	/*!
	* \ingroup utils
	* \class FixedVector
	* \brief Core class that represents an inplace vector with a compile-time capacity (and thus no allocation required)
	*/

	template<typename T, std::size_t Capacity>
	constexpr FixedVector<T, Capacity>::FixedVector() :
	m_size(0)
	{
	}

	template<typename T, std::size_t Capacity>
	constexpr FixedVector<T, Capacity>::FixedVector(std::size_t size, const T& value) :
	FixedVector()
	{
		resize(size, value);
	}

	template<typename T, std::size_t Capacity>
	constexpr FixedVector<T, Capacity>::FixedVector(const FixedVector& vec) :
	FixedVector()
	{
		for (size_type i = 0; i < vec.size(); ++i)
			push_back(vec[i]);
	}

	template<typename T, std::size_t Capacity>
	constexpr FixedVector<T, Capacity>::FixedVector(FixedVector&& vec) noexcept :
	FixedVector()
	{
		for (size_type i = 0; i < vec.size(); ++i)
			push_back(std::move(vec[i]));
	}

	template<typename T, std::size_t Capacity>
	FixedVector<T, Capacity>::~FixedVector()
	{
		clear();
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::back() -> reference
	{
		assert(!empty());
		return *data(m_size - 1);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::back() const -> const_reference
	{
		assert(!empty());
		return *data(m_size - 1);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::begin() noexcept -> iterator
	{
		return iterator(data());
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::begin() const noexcept -> const_iterator
	{
		return const_iterator(data());
	}

	template<typename T, std::size_t Capacity>
	constexpr void FixedVector<T, Capacity>::clear() noexcept
	{
		// can't use resize(0); since it will try to instantiate the default-construction part (which won't compile for classes having no default constructor)
		for (std::size_t i = 0; i < m_size; ++i)
			PlacementDestroy(data(i));

		m_size = 0;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::cbegin() const noexcept -> const_iterator
	{
		return const_iterator(data());
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::cend() const noexcept -> const_iterator
	{
		return const_iterator(data(m_size));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::crbegin() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(data(m_size));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::crend() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(data());
	}

	template<typename T, std::size_t Capacity>
	constexpr T* FixedVector<T, Capacity>::data() noexcept
	{
		return data(0);
	}

	template<typename T, std::size_t Capacity>
	constexpr T* FixedVector<T, Capacity>::data(size_type n) noexcept
	{
		return std::launder(reinterpret_cast<T*>(&m_data[0]) + n);
	}

	template<typename T, std::size_t Capacity>
	constexpr const T* FixedVector<T, Capacity>::data() const noexcept
	{
		return data(0);
	}

	template<typename T, std::size_t Capacity>
	constexpr const T* FixedVector<T, Capacity>::data(size_type n) const noexcept
	{
		return std::launder(reinterpret_cast<const T*>(&m_data[0]) + n);
	}

	template<typename T, std::size_t Capacity>
	template<typename... Args>
	constexpr auto FixedVector<T, Capacity>::emplace(const_iterator pos, Args&& ...args) -> iterator
	{
		assert(m_size < Capacity);
		assert(pos >= begin() && pos <= end());

		std::size_t index = std::distance(cbegin(), pos);
		if (pos < end())
		{
			iterator lastElement = end() - 1;
			PlacementNew(data(m_size), std::move(*lastElement));

			if (data(index) < lastElement)
				std::move_backward(data(index), data(m_size - 1), data(m_size));

			PlacementDestroy(data(index));
		}
		m_size++;

		return PlacementNew(data(index), std::forward<Args>(args)...);
	}

	template<typename T, std::size_t Capacity>
	template<typename... Args>
	constexpr auto FixedVector<T, Capacity>::emplace_back(Args&&... args) -> reference
	{
		assert(m_size < Capacity);
		return *PlacementNew(data(m_size++), std::forward<Args>(args)...);
	}

	template<typename T, std::size_t Capacity>
	constexpr bool FixedVector<T, Capacity>::empty() const noexcept
	{
		return m_size == 0;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::end() noexcept -> iterator
	{
		return iterator(data(m_size));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::end() const noexcept -> const_iterator
	{
		return const_iterator(data(m_size));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::erase(const_iterator pos) -> iterator
	{
		assert(pos < end());
		std::size_t index = std::distance(cbegin(), pos);
		std::move(begin() + index + 1, end(), begin() + index);
		pop_back();

		return iterator(data(index));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::erase(const_iterator first, const_iterator last) -> iterator
	{
		std::size_t index = std::distance(cbegin(), first);

		if (first == last)
			return begin() + index;

		assert(first < last);
		assert(first >= begin() && last <= end());

		std::size_t count = std::distance(first, last);

		std::move(begin() + index + count, end(), begin() + index);
		resize(size() - count);

		return iterator(data(index));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::front() noexcept -> reference
	{
		assert(!empty());
		return *data();
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::front() const noexcept -> const_reference
	{
		assert(!empty());
		return *data();
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::insert(const_iterator pos, const T& value) -> iterator
	{
		return emplace(pos, value);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::insert(const_iterator pos, T&& value) -> iterator
	{
		return emplace(pos, std::move(value));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::max_size() const noexcept -> size_type
	{
		return capacity();
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::push_back(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value) -> reference
	{
		assert(m_size < Capacity);
		return *PlacementNew(data(m_size++), value);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::push_back(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value) -> reference
	{
		assert(m_size < Capacity);
		return *PlacementNew(data(m_size++), std::move(value));
	}

	template<typename T, std::size_t Capacity>
	constexpr void FixedVector<T, Capacity>::pop_back()
	{
		assert(!empty());
		PlacementDestroy(data(--m_size));
	}

	template<typename T, std::size_t Capacity>
	constexpr void FixedVector<T, Capacity>::resize(size_type count)
	{
		assert(count <= Capacity);
		if (count > m_size)
		{
			for (std::size_t i = m_size; i < count; ++i)
				PlacementNew(data(i));

			m_size = count;
		}
		else if (count < m_size)
		{
			for (std::size_t i = count; i < m_size; ++i)
				PlacementDestroy(data(i));

			m_size = count;
		}
	}

	template<typename T, std::size_t Capacity>
	constexpr void FixedVector<T, Capacity>::resize(size_type count, const value_type& value)
	{
		assert(count <= Capacity);
		if (count > m_size)
		{
			for (std::size_t i = m_size; i < count; ++i)
				PlacementNew(data(i), value);

			m_size = count;
		}
		else if (count < m_size)
		{
			for (std::size_t i = count; i < m_size; ++i)
				PlacementDestroy(data(i));

			m_size = count;
		}
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::rbegin() noexcept -> reverse_iterator
	{
		return reverse_iterator(iterator(data(m_size)));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::rbegin() const noexcept -> const_reverse_iterator
	{
		return reverse_iterator(iterator(data(m_size)));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::rend() noexcept -> reverse_iterator
	{
		return reverse_iterator(iterator(data()));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::rend() const noexcept -> const_reverse_iterator
	{
		return reverse_iterator(iterator(data()));
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::size() const noexcept -> size_type
	{
		return m_size;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::operator[](size_type pos) -> reference
	{
		assert(pos < m_size);
		return *data(pos);
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::operator[](size_type pos) const -> const_reference
	{
		assert(pos < m_size);
		return *data(pos);
	}
	
	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::operator=(const FixedVector& vec) -> FixedVector&
	{
		clear();
		for (size_type i = 0; i < vec.size(); ++i)
			push_back(vec[i]);

		return *this;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::operator=(FixedVector&& vec) noexcept -> FixedVector&
	{
		clear();
		for (size_type i = 0; i < vec.size(); ++i)
			push_back(std::move(vec[i]));

		return *this;
	}

	template<typename T, std::size_t Capacity>
	constexpr auto FixedVector<T, Capacity>::capacity() noexcept -> size_type
	{
		return Capacity;
	}
}
