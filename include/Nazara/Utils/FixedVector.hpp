// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_FIXEDVECTOR_HPP
#define NAZARA_UTILS_FIXEDVECTOR_HPP

#include <Nazara/Utils/MemoryHelper.hpp>
#include <Nazara/Utils/MovablePtr.hpp>
#include <array>
#include <iterator>
#include <type_traits>

namespace Nz
{
	template<typename T, std::size_t Capacity>
	class FixedVector
	{
		public:
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
			constexpr explicit FixedVector(std::size_t size, const T& value = T{});
			constexpr FixedVector(const FixedVector& vec);
			constexpr FixedVector(FixedVector&& vec) noexcept;
			~FixedVector();

			constexpr reference back();
			constexpr const_reference back() const;

			constexpr iterator begin() noexcept;
			constexpr const_iterator begin() const noexcept;

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

			constexpr reverse_iterator rbegin() noexcept;
			constexpr const_reverse_iterator rbegin() const noexcept;

			constexpr reverse_iterator rend() noexcept;
			constexpr const_reverse_iterator rend() const noexcept;

			constexpr size_type size() const noexcept;

			constexpr reference operator[](size_type pos);
			constexpr const_reference operator[](size_type pos) const;

			constexpr FixedVector& operator=(const FixedVector& vec);
			constexpr FixedVector& operator=(FixedVector&& vec) noexcept;

			static constexpr size_type capacity() noexcept;

		private:
			alignas(T) std::array<std::byte, sizeof(T) * Capacity> m_data;
			std::size_t m_size;
	};

}

#include <Nazara/Utils/FixedVector.inl>

#endif // NAZARA_UTILS_FIXEDVECTOR_HPP
