// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_ENUMARRAY_HPP
#define NAZARA_UTILS_ENUMARRAY_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/Algorithm.hpp>
#include <array>

namespace Nz
{
	template<typename E, typename V>
	class EnumArray : public std::array<V, EnumValueCount_v<E>>
	{
		using Base = std::array<V, EnumValueCount_v<E>>;

		public:
			class kv_iterator;
			class kv_const_iterator;

			struct kv_iter_tag;

			constexpr kv_iter_tag iter_kv() noexcept;

			constexpr V& operator[](E entry) noexcept;
			constexpr const V& operator[](E entry) const noexcept;

			struct kv_iter_tag
			{
				constexpr kv_iterator begin() noexcept;
				constexpr kv_iterator end() noexcept;

				EnumArray& arrayRef;
			};
	};

	template<typename E, typename V>
	class EnumArray<E, V>::kv_iterator
	{
		friend EnumArray;

		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = std::pair<E, V&>;
			using difference_type = std::ptrdiff_t;

			constexpr kv_iterator(kv_iter_tag arrayTag, typename EnumArray::difference_type index);
			kv_iterator(const kv_iterator&) = default;
			kv_iterator(kv_iterator&&) noexcept = default;

			kv_iterator& operator=(const kv_iterator&) = default;
			kv_iterator& operator=(kv_iterator&&) noexcept = default;

			constexpr kv_iterator operator++(int) noexcept;
			constexpr kv_iterator& operator++() noexcept;

			constexpr kv_iterator operator--(int) noexcept;
			constexpr kv_iterator& operator--() noexcept;

			constexpr kv_iterator operator+(typename EnumArray::difference_type n) noexcept;
			constexpr kv_iterator operator-(typename EnumArray::difference_type n) noexcept;

			constexpr kv_iterator operator+=(typename EnumArray::difference_type n) noexcept;
			constexpr kv_iterator operator-=(typename EnumArray::difference_type n) noexcept;

			constexpr value_type operator*() const noexcept;
			constexpr value_type operator[](typename EnumArray::difference_type n) noexcept;

			constexpr bool operator==(const kv_iterator& rhs) const noexcept;
			constexpr bool operator!=(const kv_iterator& rhs) const noexcept;
			constexpr bool operator<(const kv_iterator& rhs) const noexcept;
			constexpr bool operator<=(const kv_iterator& rhs) const noexcept;
			constexpr bool operator>(const kv_iterator& rhs) const noexcept;
			constexpr bool operator>=(const kv_iterator& rhs) const noexcept;

		private:
			EnumArray& m_array;
			typename EnumArray::difference_type m_index;
	};

	template<typename E, typename V>
	typename EnumArray<E, V>::kv_iterator operator+(typename EnumArray<E, V>::difference_type n, const typename EnumArray<E, V>::kv_iterator& iterator) noexcept;

	template<typename E, typename V>
	typename EnumArray<E, V>::kv_iterator operator-(typename EnumArray<E, V>::difference_type n, const typename EnumArray<E, V>::kv_iterator& iterator) noexcept;
}

#include <NazaraUtils/EnumArray.inl>

#endif // NAZARA_UTILS_ENUMARRAY_HPP
