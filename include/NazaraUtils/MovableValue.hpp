// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_MOVABLEVALUE_HPP
#define NAZARAUTILS_MOVABLEVALUE_HPP

namespace Nz
{
	template<typename T>
	class MovableValue
	{
		public:
			MovableValue(T value = T{});
			MovableValue(const MovableValue&) = default;
			MovableValue(MovableValue&& ptr) noexcept;
			~MovableValue() = default;

			T& Get();
			const T& Get() const;

			operator T&();
			operator const T&() const;

			MovableValue& operator=(T value);
			MovableValue& operator=(const MovableValue&) = default;
			MovableValue& operator=(MovableValue&& ptr) noexcept;

		private:
			T m_value;
	};
}

#include <NazaraUtils/MovableValue.inl>

#endif // NAZARAUTILS_MOVABLEVALUE_HPP
