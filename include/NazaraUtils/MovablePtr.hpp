// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_MOVABLEPTR_HPP
#define NAZARAUTILS_MOVABLEPTR_HPP

namespace Nz
{
	template<typename T>
	class MovablePtr
	{
		public:
			MovablePtr(T* value = nullptr);
			MovablePtr(const MovablePtr&) = default;
			MovablePtr(MovablePtr&& ptr) noexcept;
			~MovablePtr() = default;

			T* Get() const;

			T* operator->() const;

			operator T*() const;

			MovablePtr& operator=(T* value);
			MovablePtr& operator=(const MovablePtr&) = default;
			MovablePtr& operator=(MovablePtr&& ptr) noexcept;

		private:
			T* m_value;
	};
}

#include <NazaraUtils/MovablePtr.inl>

#endif // NAZARAUTILS_MOVABLEPTR_HPP
