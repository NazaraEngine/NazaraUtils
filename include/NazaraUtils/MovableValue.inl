// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <utility>

namespace Nz
{
	template<typename T, T DefaultValue>
	constexpr MovableLiteral<T, DefaultValue>::MovableLiteral() :
	m_value(DefaultValue)
	{
	}

	template<typename T, T DefaultValue>
	template<typename U, typename>
	constexpr MovableLiteral<T, DefaultValue>::MovableLiteral(U&& value) :
	m_value(std::forward<U>(value))
	{
	}

	template<typename T, T DefaultValue>
	constexpr MovableLiteral<T, DefaultValue>::MovableLiteral(MovableLiteral&& val) noexcept :
	m_value(std::move(val.m_value))
	{
		val.m_value = DefaultValue;
	}

	template<typename T, T DefaultValue>
	constexpr T& MovableLiteral<T, DefaultValue>::Get() &
	{
		return m_value;
	}

	template<typename T, T DefaultValue>
	constexpr T&& MovableLiteral<T, DefaultValue>::Get() &&
	{
		return std::move(m_value);
	}

	template<typename T, T DefaultValue>
	constexpr const T& MovableLiteral<T, DefaultValue>::Get() const &
	{
		return m_value;
	}

	template<typename T, T DefaultValue>
	constexpr MovableLiteral<T, DefaultValue>::operator T&() &
	{
		return m_value;
	}

	template<typename T, T DefaultValue>
	constexpr MovableLiteral<T, DefaultValue>::operator T&&() &&
	{
		return std::move(m_value);
	}

	template<typename T, T DefaultValue>
	constexpr MovableLiteral<T, DefaultValue>::operator const T&() const &
	{
		return m_value;
	}

	template<typename T, T DefaultValue>
	template<typename U, typename>
	constexpr MovableLiteral<T, DefaultValue>& MovableLiteral<T, DefaultValue>::operator=(U&& value)
	{
		m_value = std::move(value);
		return *this;
	}

	template<typename T, T DefaultValue>
	constexpr MovableLiteral<T, DefaultValue>& MovableLiteral<T, DefaultValue>::operator=(MovableLiteral&& val) noexcept
	{
		std::swap(m_value, val.m_value);
		return *this;
	}

	/************************************************************************/ 

	template<typename T>
	template<typename U, typename>
	constexpr MovableValue<T>::MovableValue(U&& value) :
	m_value(std::forward<U>(value))
	{
	}

	template<typename T>
	constexpr MovableValue<T>::MovableValue(MovableValue&& val) noexcept :
	m_value(std::move(val.m_value))
	{
	}

	template<typename T>
	constexpr T& MovableValue<T>::Get() &
	{
		return m_value;
	}

	template<typename T>
	constexpr T&& MovableValue<T>::Get() &&
	{
		return std::move(m_value);
	}

	template<typename T>
	constexpr const T& MovableValue<T>::Get() const &
	{
		return m_value;
	}

	template<typename T>
	constexpr MovableValue<T>::operator T&() &
	{
		return m_value;
	}

	template<typename T>
	constexpr MovableValue<T>::operator T&&() &&
	{
		return std::move(m_value);
	}

	template<typename T>
	constexpr MovableValue<T>::operator const T&() const &
	{
		return m_value;
	}

	template<typename T>
	template<typename U, typename>
	constexpr MovableValue<T>& MovableValue<T>::operator=(U&& value)
	{
		m_value = std::move(value);
		return *this;
	}

	template<typename T>
	constexpr MovableValue<T>& MovableValue<T>::operator=(MovableValue&& val) noexcept
	{
		m_value = std::move(val.m_value);
		return *this;
	}
}

