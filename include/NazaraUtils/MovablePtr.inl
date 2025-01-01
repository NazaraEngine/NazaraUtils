// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <utility>

namespace Nz
{
	/*!
	* \ingroup utils
	* \class Nz::MovablePtr
	* \brief Wraps a raw (non-proprietary) to allows it to be moved implicitly
	*/

	template<typename T>
	MovablePtr<T>::MovablePtr(T* value) :
	m_value(value)
	{
	}

	template<typename T>
	MovablePtr<T>::MovablePtr(MovablePtr&& ptr) noexcept :
	m_value(ptr.m_value)
	{
		ptr.m_value = nullptr;
	}

	template<typename T>
	inline T* MovablePtr<T>::Get() const
	{
		return m_value;
	}

	template<typename T>
	T* MovablePtr<T>::operator->() const
	{
		return m_value;
	}

	template<typename T>
	MovablePtr<T>::operator T*() const
	{
		return m_value;
	}

	template<typename T>
	inline MovablePtr<T>& MovablePtr<T>::operator=(T* value)
	{
		m_value = value;

		return *this;
	}

	template<typename T>
	MovablePtr<T>& MovablePtr<T>::operator=(MovablePtr&& ptr) noexcept
	{
		std::swap(m_value, ptr.m_value);
		return *this;
	}
}

