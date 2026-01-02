// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/StringUtils.hpp>

namespace Nz
{
	/*!
	* \ingroup utils
	* \brief Converts a std::u8string to a std::string
	* \return A utf-8 string stored inside a std::string object
	*
	* \param str A UTF-8 string encoded in a std::u8string if C++20 or std::string for lower version
	*/
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20)
	std::string_view FromUtf8String(const char8_t* str)
	{
		return std::string_view(reinterpret_cast<const char*>(str));
	}

	inline std::string FromUtf8String(const std::u8string& str)
	{
		return std::string(reinterpret_cast<const char*>(str.data()), str.size());
	}

	std::string_view FromUtf8String(std::u8string_view str)
	{
		return std::string_view(reinterpret_cast<const char*>(str.data()), str.size());
	}
#else
	inline std::string FromUtf8String(const char* str)
	{
		return str; // dummy
	}

	inline std::string FromUtf8String(std::string str)
	{
		return str; // dummy
	}

	std::string_view FromUtf8String(std::string_view str)
	{
		return str; // dummy
	}
#endif

	/*!
	* \ingroup utils
	* \brief Converts a std::string to a std::u8string
	* \return A utf-8 string stored inside a std::u8string object
	*
	* \param str A UTF-8 string encoded in a std::u8string if C++20 or std::string for lower version
	*/
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20)
	std::u8string_view ToUtf8String(const char* str)
	{
		return ToUtf8String(std::string_view(str));
	}

	inline std::u8string ToUtf8String(const std::string& str)
	{
		return std::u8string(reinterpret_cast<const char8_t*>(str.data()), str.size());
	}

	std::u8string_view ToUtf8String(std::string_view str)
	{
		return std::u8string_view(reinterpret_cast<const char8_t*>(str.data()), str.size());
	}
#else
	inline std::string ToUtf8String(const char* str)
	{
		return str; // dummy
	}

	inline std::string ToUtf8String(std::string str)
	{
		return str; // dummy
	}

	inline std::string_view ToUtf8String(std::string_view str)
	{
		return str; // dummy
	}
#endif
}

