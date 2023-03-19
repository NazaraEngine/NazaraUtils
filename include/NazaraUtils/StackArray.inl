// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/StackArray.hpp>
#include <algorithm>
#include <cassert>
#include <new>
#include <utility>

namespace Nz
{
	/*!
	* \ingroup utils
	* \class Nz::StackArray
	* \brief Core class that represents a stack-allocated (if alloca is present) array
	*/

	template<typename T>
	StackArray<T>::StackArray() :
	m_size(0),
	m_ptr(nullptr)
	{
	}

	template<typename T>
	StackArray<T>::StackArray(T* stackMemory, std::size_t size) :
	m_size(size),
	m_ptr(stackMemory)
	{
		for (std::size_t i = 0; i < m_size; ++i)
			PlacementNew(&m_ptr[i]);
	}

	template<typename T>
	StackArray<T>::StackArray(T* stackMemory, std::size_t size, NoInitTag) :
	m_size(size),
	m_ptr(stackMemory)
	{
	}

	template<typename T>
	StackArray<T>::~StackArray()
	{
		for (std::size_t i = 0; i < m_size; ++i)
			PlacementDestroy(&m_ptr[i]);

		#ifndef NAZARA_ALLOCA_SUPPORT
		OperatorDelete(m_ptr);
		#endif
	}

	template<typename T>
	typename StackArray<T>::reference StackArray<T>::back()
	{
		assert(m_size != 0);
		return m_ptr[m_size - 1];
	}

	template<typename T>
	typename StackArray<T>::const_reference StackArray<T>::back() const
	{
		assert(m_size != 0);
		return m_ptr[m_size - 1];
	}

	template<typename T>
	typename StackArray<T>::iterator StackArray<T>::begin() noexcept
	{
		return iterator(&m_ptr[0]);
	}

	template<typename T>
	typename StackArray<T>::const_iterator StackArray<T>::begin() const noexcept
	{
		return const_iterator(&m_ptr[0]);
	}

	template<typename T>
	typename StackArray<T>::const_iterator StackArray<T>::cbegin() const noexcept
	{
		return const_iterator(&m_ptr[0]);
	}

	template<typename T>
	typename StackArray<T>::const_iterator StackArray<T>::cend() const noexcept
	{
		return const_iterator(&m_ptr[m_size]);
	}

	template<typename T>
	typename StackArray<T>::const_reverse_iterator StackArray<T>::crbegin() const noexcept
	{
		return const_reverse_iterator(&m_ptr[m_size]);
	}

	template<typename T>
	typename StackArray<T>::const_reverse_iterator StackArray<T>::crend() const noexcept
	{
		return const_reverse_iterator(&m_ptr[0]);
	}

	template<typename T>
	T* StackArray<T>::data() noexcept
	{
		return m_ptr;
	}

	template<typename T>
	const T* StackArray<T>::data() const noexcept
	{
		return m_ptr;
	}

	template<typename T>
	bool StackArray<T>::empty() const noexcept
	{
		return m_size == 0;
	}

	template<typename T>
	typename StackArray<T>::iterator StackArray<T>::end() noexcept
	{
		return iterator(&m_ptr[m_size]);
	}

	template<typename T>
	typename StackArray<T>::const_iterator StackArray<T>::end() const noexcept
	{
		return const_iterator(&m_ptr[m_size]);
	}

	template<typename T>
	void StackArray<T>::fill(const T& value)
	{
		std::fill(begin(), end(), value);
	}

	template<typename T>
	typename StackArray<T>::reference StackArray<T>::front() noexcept
	{
		return m_ptr[0];
	}

	template<typename T>
	typename StackArray<T>::const_reference StackArray<T>::front() const noexcept
	{
		return m_ptr[0];
	}

	template<typename T>
	typename StackArray<T>::size_type StackArray<T>::max_size() const noexcept
	{
		return size();
	}

	template<typename T>
	typename StackArray<T>::reverse_iterator StackArray<T>::rbegin() noexcept
	{
		return reverse_iterator(&m_ptr[m_size]);
	}

	template<typename T>
	typename StackArray<T>::const_reverse_iterator StackArray<T>::rbegin() const noexcept
	{
		return reverse_iterator(&m_ptr[m_size]);
	}

	template<typename T>
	typename StackArray<T>::reverse_iterator StackArray<T>::rend() noexcept
	{
		return reverse_iterator(&m_ptr[0]);
	}

	template<typename T>
	typename StackArray<T>::const_reverse_iterator StackArray<T>::rend() const noexcept
	{
		return reverse_iterator(&m_ptr[0]);
	}

	template<typename T>
	typename StackArray<T>::size_type StackArray<T>::size() const noexcept
	{
		return m_size;
	}

	template<typename T>
	typename StackArray<T>::reference StackArray<T>::operator[](size_type pos)
	{
		assert(pos < m_size);
		return m_ptr[pos];
	}

	template<typename T>
	typename StackArray<T>::const_reference StackArray<T>::operator[](size_type pos) const
	{
		assert(pos < m_size);
		return m_ptr[pos];
	}
}
