// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_HASH_HPP
#define NAZARA_UTILS_HASH_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <string_view>

namespace Nz
{
	template<typename T> void HashCombine(std::size_t& seed, const T& v);
	template<typename T, typename... Args> std::size_t HashCombine(const T& v, const Args&... args);

	template<typename T>
	struct HashFunctor
	{
		template<typename... Args> std::size_t operator()(Args&&... args);
	};

	// Cyclic Redundancy Check (CRC) - 32bits
	constexpr UInt32 CRC32(const char* str) noexcept;
	constexpr UInt32 CRC32(std::string_view str) noexcept;
	constexpr UInt32 CRC32(const UInt8* data, std::size_t size) noexcept;
	template<std::size_t N> constexpr UInt32 CRC32(const char(&str)[N]) noexcept;

	struct CRC32Hash
	{
		template<typename... Args> UInt32 operator()(Args&&... args);
	};

	// Fowler–Noll–Vo (FNV) 1a - 32bits
	constexpr UInt32 FNV1a32(const char* str) noexcept;
	constexpr UInt32 FNV1a32(std::string_view str) noexcept;
	constexpr UInt32 FNV1a32(const UInt8* data, std::size_t size) noexcept;
	template<std::size_t N> constexpr UInt32 FNV1a32(const char(&str)[N]) noexcept;

	struct FNV1a32Hash
	{
		template<typename... Args> UInt32 operator()(Args&&... args);
	};

	// Fowler–Noll–Vo (FNV) 1a - 64bits
	constexpr UInt64 FNV1a64(const char* str) noexcept;
	constexpr UInt64 FNV1a64(std::string_view str) noexcept;
	constexpr UInt64 FNV1a64(const UInt8* data, std::size_t size) noexcept;
	template<std::size_t N> constexpr UInt64 FNV1a64(const char(&str)[N]) noexcept;

	struct FNV1a64Hash
	{
		template<typename... Args> UInt64 operator()(Args&&... args);
	};
}

#include <NazaraUtils/Hash.inl>

#endif // NAZARA_UTILS_HASH_HPP
