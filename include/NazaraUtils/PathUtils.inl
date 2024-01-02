// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/PathUtils.hpp>

namespace Nz
{
	/*!
	* \ingroup utils
	* \brief Converts a std::filesystem::path to a UTF-8 encoded std::string
	* \return A utf-8 string containing the path
	*
	* \param path Path
	*/
	inline std::string PathToString(const std::filesystem::path& path)
	{
		// Android NDK 25c supports C++20 but its std::filesystem is not conforming (stills returns std::string)
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20) && (!defined(NAZARA_PLATFORM_ANDROID) || NAZARA_CHECK_NDK_VER(26))
		return FromUtf8String(path.generic_u8string());
#else
		return path.generic_u8string();
#endif
	}

	/*!
	* \ingroup utils
	* \brief Builds a path from a UTF-8 string
	* \return A path built from the UTF-8 string
	*
	* \param path A UTF-8 string representing the path
	*/
	inline std::filesystem::path Utf8Path(std::string_view path)
	{
		// Android NDK 25c supports C++20 but its std::filesystem is not conforming
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20) && (!defined(NAZARA_PLATFORM_ANDROID) || NAZARA_CHECK_NDK_VER(26))
		return std::filesystem::path(ToUtf8String(path));
#else
		return std::filesystem::u8path(path);
#endif
	}
}

