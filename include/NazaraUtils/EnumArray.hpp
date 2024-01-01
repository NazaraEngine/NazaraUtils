// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_ENUMARRAY_HPP
#define NAZARAUTILS_ENUMARRAY_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/Algorithm.hpp>
#include <array>

namespace Nz
{
	template<typename E, typename V, bool Const>
	class EnumArrayKvIterator;

	template<typename E, typename V>
	class EnumArray : public std::array<V, EnumValueCount_v<E>>
	{
		using Base = std::array<V, EnumValueCount_v<E>>;

		public:
			using kv_iterator = EnumArrayKvIterator<E, V, false>;
			using kv_const_iterator = EnumArrayKvIterator<E, V, true>;

			struct kv_iter_tag;
			struct kv_const_iter_tag;

			constexpr kv_iter_tag iter_kv() noexcept;
			constexpr kv_const_iter_tag iter_kv() const noexcept;

			constexpr V& operator[](E entry) noexcept;
			constexpr const V& operator[](E entry) const noexcept;

			struct kv_iter_tag
			{
				constexpr kv_iterator begin() noexcept;
				constexpr kv_iterator end() noexcept;

				EnumArray& arrayRef;
			};

			struct kv_const_iter_tag
			{
				constexpr kv_const_iterator begin() const noexcept;
				constexpr kv_const_iterator end() const noexcept;

				const EnumArray& arrayRef;
			};
	};

	template<typename E, typename V, bool Const>
	class EnumArrayKvIterator
	{
		using Array = EnumArray<E, V>;
		friend Array;

		public:
			using kv_tag = std::conditional_t<Const, typename Array::kv_const_iter_tag, typename Array::kv_iter_tag>;
			using iterator_category = std::random_access_iterator_tag;
			using value_type = std::pair<E, std::conditional_t<Const, const V&, V&>>;
			using difference_type = typename Array::difference_type;

			constexpr EnumArrayKvIterator(kv_tag arrayTag, difference_type index);
			EnumArrayKvIterator(const EnumArrayKvIterator&) = default;
			EnumArrayKvIterator(EnumArrayKvIterator&&) noexcept = default;

			EnumArrayKvIterator& operator=(const EnumArrayKvIterator&) = default;
			EnumArrayKvIterator& operator=(EnumArrayKvIterator&&) noexcept = default;

			constexpr EnumArrayKvIterator operator++(int) noexcept;
			constexpr EnumArrayKvIterator& operator++() noexcept;

			constexpr EnumArrayKvIterator operator--(int) noexcept;
			constexpr EnumArrayKvIterator& operator--() noexcept;

			constexpr EnumArrayKvIterator operator+(difference_type n) noexcept;
			constexpr EnumArrayKvIterator operator-(difference_type n) noexcept;

			constexpr EnumArrayKvIterator operator+=(difference_type n) noexcept;
			constexpr EnumArrayKvIterator operator-=(difference_type n) noexcept;

			constexpr value_type operator*() const noexcept;
			constexpr value_type operator[](difference_type n) const noexcept;

			constexpr bool operator==(const EnumArrayKvIterator& rhs) const noexcept;
			constexpr bool operator!=(const EnumArrayKvIterator& rhs) const noexcept;
			constexpr bool operator<(const EnumArrayKvIterator& rhs) const noexcept;
			constexpr bool operator<=(const EnumArrayKvIterator& rhs) const noexcept;
			constexpr bool operator>(const EnumArrayKvIterator& rhs) const noexcept;
			constexpr bool operator>=(const EnumArrayKvIterator& rhs) const noexcept;

		private:
			std::conditional_t<Const, const Array, Array>& m_array;
			difference_type m_index;
	};

	template<typename E, typename V, bool Const>
	EnumArrayKvIterator<E, V, Const> operator+(typename EnumArrayKvIterator<E, V, Const>::difference_type n, const EnumArrayKvIterator<E, V, Const>& iterator) noexcept;
}

#include <NazaraUtils/EnumArray.inl>

#endif // NAZARAUTILS_ENUMARRAY_HPP
