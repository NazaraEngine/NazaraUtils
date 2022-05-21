// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_MOVABLEPTR_HPP
#define NAZARA_UTILS_MOVABLEPTR_HPP

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

#include <Nazara/Utils/MovablePtr.inl>

#endif // NAZARA_UTILS_MOVABLEPTR_HPP
