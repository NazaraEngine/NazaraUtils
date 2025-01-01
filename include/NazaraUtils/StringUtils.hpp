// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_STRING_UTILS_HPP
#define NAZARAUTILS_STRING_UTILS_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <string>
#include <string_view>

namespace Nz
{
	// String utils
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20)
	inline std::string_view FromUtf8String(const char8_t* str);
	inline std::string FromUtf8String(const std::u8string& str);
	inline std::string_view FromUtf8String(std::u8string_view str);
#else
	inline std::string FromUtf8String(const char* str);
	inline std::string FromUtf8String(std::string str);
	inline std::string_view FromUtf8String(std::string_view str);
#endif

#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20)
	inline std::u8string_view ToUtf8String(const char* str);
	inline std::u8string ToUtf8String(const std::string& str);
	inline std::u8string_view ToUtf8String(std::string_view str);
#else
	inline std::string ToUtf8String(const char* str);
	inline std::string ToUtf8String(std::string str);
	inline std::string_view ToUtf8String(std::string_view str);
#endif
}

#include <NazaraUtils/StringUtils.inl>

#endif // NAZARAUTILS_STRING_UTILS_HPP
