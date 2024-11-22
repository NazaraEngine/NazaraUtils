// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_FIXEDVECTOR_HPP
#define NAZARAUTILS_FIXEDVECTOR_HPP

#include <NazaraUtils/MemoryHelper.hpp>
#include <NazaraUtils/MathUtils.hpp>
#include <NazaraUtils/MovablePtr.hpp>
#include <array>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <vector>

namespace Nz
{
	namespace Detail
	{
		template<typename T, typename Fallback>
		class FixedVectorBase;
	}

	template<typename T, std::size_t Capacity, typename Fallback = void>
	class FixedVector : public Detail::FixedVectorBase<T, Fallback>
	{
		static_assert(Capacity > 0);
		using Base = Detail::FixedVectorBase<T, Fallback>;

		public:
			static constexpr std::size_t FixedCapacity = Capacity;
			using value_type = T;
			using const_iterator = const value_type*;
			using const_pointer = const value_type*;
			using const_reference = const value_type&;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;
			using difference_type = std::ptrdiff_t;
			using iterator = value_type*;
			using pointer = value_type*;
			using reference = value_type&;
			using reverse_iterator = std::reverse_iterator<iterator>;
			using size_type = std::size_t;

			constexpr FixedVector();
			constexpr explicit FixedVector(size_type size, const T& value = T{});
			template<typename InputIt> constexpr FixedVector(InputIt first, InputIt last);
			constexpr FixedVector(std::initializer_list<T> init);
			constexpr FixedVector(const FixedVector& vec);
			constexpr FixedVector(FixedVector&& vec) noexcept;
			~FixedVector();

			constexpr reference back();
			constexpr const_reference back() const;

			constexpr iterator begin() noexcept;
			constexpr const_iterator begin() const noexcept;

			constexpr size_type capacity() const noexcept;

			constexpr void clear() noexcept;

			constexpr const_iterator cbegin() const noexcept;
			constexpr const_iterator cend() const noexcept;
			constexpr const_reverse_iterator crbegin() const noexcept;
			constexpr const_reverse_iterator crend() const noexcept;

			constexpr T* data() noexcept;
			constexpr T* data(size_type n) noexcept;
			constexpr const T* data() const noexcept;
			constexpr const T* data(size_type n) const noexcept;

			template<typename... Args> 
			constexpr iterator emplace(const_iterator pos, Args&&... args);

			template<typename... Args>
			constexpr reference emplace_back(Args&&... args);

			constexpr bool empty() const noexcept;

			constexpr iterator end() noexcept;
			constexpr const_iterator end() const noexcept;

			constexpr iterator erase(const_iterator pos);
			constexpr iterator erase(const_iterator first, const_iterator last);

			constexpr reference front() noexcept;
			constexpr const_reference front() const noexcept;

			constexpr iterator insert(const_iterator pos, const T& value);
			constexpr iterator insert(const_iterator pos, T&& value);

			constexpr size_type max_size() const noexcept;

			constexpr reference push_back(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value);
			constexpr reference push_back(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value);

			constexpr void pop_back();

			constexpr void resize(size_type count);
			constexpr void resize(size_type count, const value_type& value);

			constexpr void reserve(size_type count);

			constexpr reverse_iterator rbegin() noexcept;
			constexpr const_reverse_iterator rbegin() const noexcept;

			constexpr reverse_iterator rend() noexcept;
			constexpr const_reverse_iterator rend() const noexcept;

			constexpr size_type size() const noexcept;

			constexpr reference operator[](size_type pos);
			constexpr const_reference operator[](size_type pos) const;

			constexpr FixedVector& operator=(const FixedVector& vec);
			constexpr FixedVector& operator=(FixedVector&& vec) noexcept;

		private:
			constexpr bool CheckFallbackOnGrow();
			constexpr bool IsUsingFallback() const;
			constexpr void MoveStorageToFallback(std::size_t capacity);

			static constexpr std::size_t FallbackInUse = Nz::MaxValue();

			std::size_t m_size;
			alignas(T) std::array<std::byte, sizeof(T) * Capacity> m_data;
	};

	template<typename T, std::size_t Capacity>
	using HybridVector = FixedVector<T, Capacity, std::vector<T>>;
}

#include <NazaraUtils/FixedVector.inl>

#endif // NAZARAUTILS_FIXEDVECTOR_HPP
