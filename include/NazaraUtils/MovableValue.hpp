// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_MOVABLEVALUE_HPP
#define NAZARAUTILS_MOVABLEVALUE_HPP

#include <type_traits>

namespace Nz
{
	template<typename T, T DefaultValue>
	class MovableLiteral
	{
		public:
			constexpr MovableLiteral();
			template<typename U, typename = std::enable_if_t<std::is_constructible_v<T, U>>> constexpr MovableLiteral(U&& value);
			constexpr MovableLiteral(const MovableLiteral&) = default;
			constexpr MovableLiteral(MovableLiteral&& val) noexcept;
			~MovableLiteral() = default;

			constexpr T& Get() &;
			constexpr T&& Get() &&;
			constexpr const T& Get() const &;

			constexpr operator T&() &;
			constexpr operator T&&() &&;
			constexpr operator const T&() const &;

			template<typename U, typename = std::enable_if_t<std::is_assignable_v<T&, U>>> constexpr MovableLiteral& operator=(U&& value);
			constexpr MovableLiteral& operator=(const MovableLiteral&) = default;
			constexpr MovableLiteral& operator=(MovableLiteral&& ptr) noexcept;

		private:
			T m_value;
	};

	template<typename T>
	class MovableValue
	{
		public:
			constexpr MovableValue() = default;
			template<typename U, typename = std::enable_if_t<std::is_constructible_v<T, U>>> constexpr MovableValue(U&& value);
			constexpr MovableValue(const MovableValue&) = default;
			constexpr MovableValue(MovableValue&& val) noexcept;
			~MovableValue() = default;

			constexpr T& Get() &;
			constexpr T&& Get() &&;
			constexpr const T& Get() const &;

			constexpr operator T&() &;
			constexpr operator T&&() &&;
			constexpr operator const T&() const &;

			template<typename U, typename = std::enable_if_t<std::is_assignable_v<T&, U>>> constexpr MovableValue& operator=(U&& value);
			constexpr MovableValue& operator=(const MovableValue&) = default;
			constexpr MovableValue& operator=(MovableValue&& val) noexcept;

		private:
			T m_value;
	};
}

#include <NazaraUtils/MovableValue.inl>

#endif // NAZARAUTILS_MOVABLEVALUE_HPP
