// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <cassert>
#include <new>
#include <utility>

namespace Nz
{
	/*!
	* \ingroup utils
	* \fn Nz::MemoryHelper
	* \brief Core functions that helps the handle of memory in the engine
	*/

	/*!
	* \brief Constructs the object inplace
	* \return Pointer to the constructed object
	*
	* \param ptr Pointer to raw memory allocated
	* \param args Arguments for the constructor
	*/
	template<typename T, typename... Args>
	constexpr T* PlacementNew(T* ptr, Args&&... args)
	{
		return new (ptr) T(std::forward<Args>(args)...);
	}

	/*!
	* \brief Calls the object destructor explicitly
	*
	* \param ptr Pointer to a previously constructed pointer on raw memory
	*
	* \remark This does not deallocate memory, and is a no-op on a null pointer
	*/
	template<typename T>
	constexpr void PlacementDestroy(T* ptr)
	{
		if (ptr)
			ptr->~T();
	}
}
