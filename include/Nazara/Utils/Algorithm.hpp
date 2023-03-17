// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_ALGORITHM_HPP
#define NAZARA_UTILS_ALGORITHM_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Utils/TypeTag.hpp>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace Nz
{
	// Math constants
	template<typename T> constexpr T HalfPi = T(1.5707963267948966192313216916398);
	template<typename T> constexpr T Pi     = T(3.1415926535897932384626433832795);
	template<typename T> constexpr T Sqrt2  = T(1.4142135623730950488016887242097);
	template<typename T> constexpr T Sqrt3  = T(1.7320508075688772935274463415059);
	template<typename T> constexpr T Sqrt5  = T(2.2360679774997896964091736687313);


	// Math utils
	template<typename T> constexpr T Approach(T value, T objective, T increment);
	template<typename T> constexpr T Clamp(T value, T min, T max);
	template<typename T> T ClearBit(T number, T bit);
	template<typename T> constexpr std::size_t CountBits(T value);
	template<typename T> constexpr T DegreeToRadian(T degrees);
	template<typename T> constexpr T GetNearestPowerOfTwo(T number);
	template<typename T> constexpr unsigned int IntegralLog2(T number);
	template<typename T> constexpr unsigned int IntegralLog2Pot(T pot);
	template<typename T> constexpr T IntegralPow(T base, unsigned int exponent);
	template<typename T, typename T2> constexpr T Lerp(const T& from, const T& to, const T2& interpolation);
	template<typename T> constexpr T MultiplyAdd(T x, T y, T z);
	template<typename T> constexpr bool NumberEquals(T a, T b);
	template<typename T> constexpr bool NumberEquals(T a, T b, T maxDifference);
	template<typename T> constexpr T RadianToDegree(T radians);
	template<typename T> T SetBit(T number, T bit);
	template<typename T> bool TestBit(T number, T bit);
	template<typename T> T ToggleBit(T number, T bit);

	// Path utils
	inline std::string PathToString(const std::filesystem::path& path);
	inline std::filesystem::path Utf8Path(std::string_view path);

	// String utils
#if NAZARA_CPP_VER >= NAZARA_CPP20 && !defined(NAZARA_PLATFORM_ANDROID)
	inline std::string FromUtf8String(const std::u8string& str);
	inline std::string_view FromUtf8String(std::u8string_view str);
	inline std::string_view FromUtf8String(const char8_t* str);
#else
	inline std::string FromUtf8String(std::string str);
	inline std::string_view FromUtf8String(std::string_view str);
#endif

#if NAZARA_CPP_VER >= NAZARA_CPP20 && !defined(NAZARA_PLATFORM_ANDROID)
	inline std::u8string ToUtf8String(const std::string& str);
	inline std::u8string_view ToUtf8String(std::string_view str);
	inline std::u8string_view ToUtf8String(const char* str);
#else
	inline std::string ToUtf8String(std::string str);
	inline std::string_view ToUtf8String(std::string_view str);
#endif

	// Generic utils
	template<typename T> decltype(auto) AccessByOffset(void* basePtr, std::size_t offset);
	template<typename T> decltype(auto) AccessByOffset(const void* basePtr, std::size_t offset);
	template<typename T> constexpr T Align(T offset, T alignment);
	template<typename T> constexpr T AlignPow2(T offset, T alignment);
	template<typename T> constexpr std::size_t BitCount();
	template<typename T, std::size_t N> constexpr std::size_t CountOf(T(&name)[N]) noexcept;
	template<typename T> std::size_t CountOf(const T& c);
	constexpr UInt32 CRC32(const UInt8* data, std::size_t size) noexcept;
	constexpr UInt32 CRC32(const char* str) noexcept;
	constexpr UInt32 CRC32(const std::string_view& str) noexcept;
	template<std::size_t N> constexpr std::size_t CountOf(const char(&str)[N]) noexcept;
	template<typename T> void HashCombine(std::size_t& seed, const T& v);
	template<typename T> bool IsPowerOfTwo(T value);
	template<typename M, typename T> auto& Retrieve(M& map, const T& key);
	template<typename M, typename T> const auto& Retrieve(const M& map, const T& key);
	template<typename T> T ReverseBits(T integer);
	template<typename To, typename From> To SafeCast(From&& value);
	template<typename T, typename U>std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& ptr);
	template<typename T> constexpr auto UnderlyingCast(T value) -> std::underlying_type_t<T>;

	template<typename T>
	struct AlwaysFalse : std::false_type {};

	// Helper for std::visit
	template<typename... Ts> struct Overloaded : Ts...
	{
		using Ts::operator()...;
	};

	template<typename... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

	template<typename... Args>
	struct OverloadResolver
	{
		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...)) const noexcept
		{
			return ptr;
		}

		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...) const) const noexcept
		{
			return ptr;
		}

		template<typename R>
		constexpr auto operator()(R(*ptr)(Args...)) const noexcept
		{
			return ptr;
		}
	};

	template<typename... Args> constexpr OverloadResolver<Args...> Overload = {};

	template<typename T>
	struct PointedType
	{
		using type = void; //< FIXME: I can't make SFINAE work
	};

	template<typename T>
	using Pointer = T*;
}

#include <Nazara/Utils/Algorithm.inl>

#endif // NAZARA_UTILS_ALGORITHM_HPP
