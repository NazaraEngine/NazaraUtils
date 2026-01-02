// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_ALGORITHM_HPP
#define NAZARAUTILS_ALGORITHM_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/ConstantEvaluated.hpp>
#include <NazaraUtils/TypeTraits.hpp>
#include <cmath>
#include <memory>
#include <type_traits>

#if __has_include(<version>)
#include <version>
#endif

// bit_cast support
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806L
	#define NAZARA_HAS_CONSTEXPR_BITCAST
	#define NAZARA_HAS_CONSTEXPR_BITCAST_STD
	#define NAZARA_CONSTEXPR_BITCAST constexpr
#elif NAZARA_CHECK_MSVC_VER(1927) || NAZARA_CHECK_CLANG_VER(1400) || NAZARA_CHECK_GCC_VER(1100)
	#define NAZARA_HAS_CONSTEXPR_BITCAST
	#define NAZARA_HAS_CONSTEXPR_BITCAST_BUILTIN
	#define NAZARA_CONSTEXPR_BITCAST constexpr
#else
	#define NAZARA_CONSTEXPR_BITCAST
#endif

namespace Nz
{
	// Generic utils

	template<typename T> [[nodiscard]] decltype(auto) AccessByOffset(void* basePtr, std::size_t offset) noexcept;
	template<typename T> [[nodiscard]] decltype(auto) AccessByOffset(const void* basePtr, std::size_t offset) noexcept;
	template<typename To, typename From> [[nodiscard]] NAZARA_CONSTEXPR_BITCAST To BitCast(const From& value) noexcept;
	template<typename T, std::size_t N> [[nodiscard]] constexpr std::size_t CountOf(T(&name)[N]) noexcept;
	template<typename T> [[nodiscard]] constexpr std::size_t CountOf(const T& c) noexcept;
	template<std::size_t N> [[nodiscard]] constexpr std::size_t CountOf(const char(&str)[N]) noexcept;
	template<typename P, typename T> [[nodiscard]] NAZARA_CONSTEXPR_BITCAST P IntegerToPointer(T ptrAsInt) noexcept;
	template<typename T, typename P> [[nodiscard]] NAZARA_CONSTEXPR_BITCAST T PointerToInteger(P* ptr) noexcept;
	template<typename M, typename T> [[nodiscard]] NAZARA_CONSTEXPR20 auto& Retrieve(M& map, const T& key) noexcept;
	template<typename M, typename T> [[nodiscard]] NAZARA_CONSTEXPR20 const auto& Retrieve(const M& map, const T& key) noexcept;
	template<typename To, typename From> [[nodiscard]] NAZARA_CONSTEXPR20 To SafeCast(From&& value) noexcept;
	template<typename From> [[nodiscard]] NAZARA_CONSTEXPR20 auto SafeCaster(From&& value) noexcept;
	template<typename T, typename U> [[nodiscard]] std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& ptr) noexcept;
	template<typename T> [[nodiscard]] constexpr auto UnderlyingCast(T value) noexcept -> std::underlying_type_t<T>;
}

#include <NazaraUtils/Algorithm.inl>

#endif // NAZARAUTILS_ALGORITHM_HPP
