// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_TYPETAG_HPP
#define NAZARAUTILS_TYPETAG_HPP

namespace Nz
{
	template<typename T>
	struct TypeTag
	{
		using Type = T;
	};
}

#endif // NAZARAUTILS_TYPETAG_HPP
