// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <cassert>

namespace Nz
{
	template<typename E, typename V>
	constexpr auto EnumArray<E, V>::iter_kv() noexcept -> kv_iter_tag
	{
		return kv_iter_tag{ *this };
	}

	template<typename E, typename V>
	constexpr auto EnumArray<E, V>::iter_kv() const noexcept -> kv_const_iter_tag
	{
		return kv_const_iter_tag{ *this };
	}

	template<typename E, typename V>
	constexpr auto EnumArray<E, V>::operator[](E entry) noexcept -> V&
	{
		return Base::operator[](UnderlyingCast(entry));
	}

	template<typename E, typename V>
	constexpr auto EnumArray<E, V>::operator[](E entry) const noexcept -> const V&
	{
		return Base::operator[](UnderlyingCast(entry));
	}


	template<typename E, typename V> 
	constexpr typename EnumArray<E, V>::kv_iterator EnumArray<E, V>::kv_iter_tag::begin() noexcept
	{
		return EnumArray<E, V>::kv_iterator(*this, 0);
	}

	template<typename E, typename V> 
	constexpr typename EnumArray<E, V>::kv_iterator EnumArray<E, V>::kv_iter_tag::end() noexcept
	{
		return EnumArray<E, V>::kv_iterator(*this, arrayRef.size());
	}

	template<typename E, typename V>
	constexpr typename EnumArray<E, V>::kv_const_iterator EnumArray<E, V>::kv_const_iter_tag::begin() const noexcept
	{
		return EnumArray<E, V>::kv_const_iterator(*this, 0);
	}

	template<typename E, typename V>
	constexpr typename EnumArray<E, V>::kv_const_iterator EnumArray<E, V>::kv_const_iter_tag::end() const noexcept
	{
		return EnumArray<E, V>::kv_const_iterator(*this, arrayRef.size());
	}


	template<typename E, typename V, bool Const>
	constexpr EnumArrayKvIterator<E, V, Const>::EnumArrayKvIterator(kv_tag arrayTag, difference_type index) :
	m_array(arrayTag.arrayRef),
	m_index(index)
	{
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator++(int) noexcept -> EnumArrayKvIterator
	{
		EnumArrayKvIterator it(*this);
		++m_index;

		return it;
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator++() noexcept -> EnumArrayKvIterator&
	{
		++m_index;
		return *this;
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator--(int) noexcept -> EnumArrayKvIterator
	{
		EnumArrayKvIterator it(*this);
		--m_index;

		return it;
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator--() noexcept -> EnumArrayKvIterator&
	{
		--m_index;
		return *this;
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator+(difference_type n) noexcept -> EnumArrayKvIterator
	{
		return EnumArrayKvIterator(m_array, m_index + n);
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator-(difference_type n) noexcept -> EnumArrayKvIterator
	{
		return EnumArrayKvIterator(m_array, m_index - n);
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator+=(difference_type n) noexcept -> EnumArrayKvIterator
	{
		m_index += n;
		return *this;
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator-=(difference_type n) noexcept -> EnumArrayKvIterator
	{
		m_index -= n;
		return *this;
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator*() const noexcept -> value_type
	{
		E enumKey = static_cast<E>(m_index);
		return std::make_pair(enumKey, std::ref(m_array[enumKey]));
	}

	template<typename E, typename V, bool Const>
	constexpr auto EnumArrayKvIterator<E, V, Const>::operator[](difference_type n) const noexcept -> value_type
	{
		E enumKey = static_cast<E>(m_index + n);
		return std::make_pair(enumKey, std::ref(m_array[enumKey]));
	}

	template<typename E, typename V, bool Const>
	constexpr bool EnumArrayKvIterator<E, V, Const>::operator==(const EnumArrayKvIterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index == rhs.m_index;
	}
	
	template<typename E, typename V, bool Const>
	constexpr bool EnumArrayKvIterator<E, V, Const>::operator!=(const EnumArrayKvIterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index != rhs.m_index;
	}

	template<typename E, typename V, bool Const>
	constexpr bool EnumArrayKvIterator<E, V, Const>::operator<(const EnumArrayKvIterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index < rhs.m_index;
	}

	template<typename E, typename V, bool Const>
	constexpr bool EnumArrayKvIterator<E, V, Const>::operator<=(const EnumArrayKvIterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index <= rhs.m_index;
	}

	template<typename E, typename V, bool Const>
	constexpr bool EnumArrayKvIterator<E, V, Const>::operator>(const EnumArrayKvIterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index > rhs.m_index;
	}

	template<typename E, typename V, bool Const>
	constexpr bool EnumArrayKvIterator<E, V, Const>::operator>=(const EnumArrayKvIterator& rhs) const noexcept
	{
		assert(&m_array == &rhs.m_array);
		return m_index >= rhs.m_index;
	}

	template<typename E, typename V, bool Const>
	EnumArrayKvIterator<E, V, Const> operator+(typename EnumArrayKvIterator<E, V, Const>::difference_type n, const EnumArrayKvIterator<E, V, Const>& iterator) noexcept
	{
		return iterator + n;
	}
}
