// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <cassert>

namespace Nz
{
	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::iter_kv() noexcept -> kv_iter_tag
	{
		return kv_iter_tag{ *this };
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::operator[](E entry) noexcept -> V&
	{
		return Base::operator[](UnderlyingCast(entry));
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::operator[](E entry) const noexcept -> const V&
	{
		return Base::operator[](UnderlyingCast(entry));
	}


	template<typename E, typename V> 
	constexpr typename EnumMap<E, V>::kv_iterator EnumMap<E, V>::kv_iter_tag::begin() noexcept
	{
		return EnumMap<E, V>::kv_iterator(*this, 0);
	}

	template<typename E, typename V> 
	constexpr typename EnumMap<E, V>::kv_iterator EnumMap<E, V>::kv_iter_tag::end() noexcept
	{
		return EnumMap<E, V>::kv_iterator(*this, arrayRef.size());
	}


	template<typename E, typename V>
	constexpr EnumMap<E, V>::kv_iterator::kv_iterator(kv_iter_tag arrayTag, typename EnumMap::difference_type index) :
	m_array(arrayTag.arrayRef),
	m_index(index)
	{
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator++(int) noexcept -> kv_iterator
	{
		kv_iterator it(*this);
		++m_index;

		return it;
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator++() noexcept -> kv_iterator&
	{
		++m_index;
		return *this;
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator--(int) noexcept -> kv_iterator
	{
		kv_iterator it(*this);
		--m_index;

		return it;
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator--() noexcept -> kv_iterator&
	{
		--m_index;
		return *this;
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator+(typename EnumMap::difference_type n) noexcept -> kv_iterator
	{
		return kv_iterator(m_array, m_index + n);
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator-(typename EnumMap::difference_type n) noexcept -> kv_iterator
	{
		return kv_iterator(m_array, m_index - n);
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator+=(typename EnumMap::difference_type n) noexcept -> kv_iterator
	{
		m_index += n;
		return *this;
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator-=(typename EnumMap::difference_type n) noexcept -> kv_iterator
	{
		m_index -= n;
		return *this;
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator*() const noexcept -> value_type
	{
		E enumKey = static_cast<E>(m_index);
		return std::make_pair(enumKey, std::ref(m_array[enumKey]));
	}

	template<typename E, typename V>
	constexpr auto EnumMap<E, V>::kv_iterator::operator[](typename EnumMap::difference_type n) noexcept -> value_type
	{
		E enumKey = static_cast<E>(m_index + n);
		return std::make_pair(enumKey, std::ref(m_array[enumKey]));
	}

	template<typename E, typename V>
	constexpr bool EnumMap<E, V>::kv_iterator::operator==(const kv_iterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index == rhs.m_index;
	}
	
	template<typename E, typename V>
	constexpr bool EnumMap<E, V>::kv_iterator::operator!=(const kv_iterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index != rhs.m_index;
	}

	template<typename E, typename V>
	constexpr bool EnumMap<E, V>::kv_iterator::operator<(const kv_iterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index < rhs.m_index;
	}

	template<typename E, typename V>
	constexpr bool EnumMap<E, V>::kv_iterator::operator<=(const kv_iterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index <= rhs.m_index;
	}

	template<typename E, typename V>
	constexpr bool EnumMap<E, V>::kv_iterator::operator>(const kv_iterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index > rhs.m_index;
	}

	template<typename E, typename V>
	constexpr bool EnumMap<E, V>::kv_iterator::operator>=(const kv_iterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index >= rhs.m_index;
	}
}
