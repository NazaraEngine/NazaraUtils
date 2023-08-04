// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <limits>
#include <utility>

#ifdef NAZARA_COMPILER_MSVC

	#pragma intrinsic(_BitScanForward)

#if defined(NAZARA_ARCH_aarch64) || defined(NAZARA_ARCH_x86_64)

	#pragma intrinsic(_BitScanForward64)

#endif

#if defined(NAZARA_ARCH_x86) || defined(NAZARA_ARCH_x86_64)

#include <intrin.h>

#endif

#endif

namespace Nz
{
	namespace Detail
	{
		// http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html
		template<typename F, typename Tuple, size_t... S>
		decltype(auto) ApplyImplFunc(F&& fn, Tuple&& t, std::index_sequence<S...>)
		{
			return std::forward<F>(fn)(std::get<S>(std::forward<Tuple>(t))...);
		}

		template<typename O, typename F, typename Tuple, size_t... S>
		decltype(auto) ApplyImplMethod(O& object, F&& fn, Tuple&& t, std::index_sequence<S...>)
		{
			return (object .* std::forward<F>(fn))(std::get<S>(std::forward<Tuple>(t))...);
		}

		constexpr UInt8 BitReverseTable256[256] =
		{
			0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
			0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
			0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
			0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
			0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
			0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
			0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
			0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
			0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
			0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
			0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
			0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
			0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
			0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
			0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
			0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
		};

		// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
		constexpr unsigned int s_MultiplyDeBruijnBitPosition[32] =
		{
			0,  9,  1, 10, 13, 21,  2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
			8, 12, 20, 28, 15, 17, 24,  7, 19, 27, 23,  6, 26,  5, 4, 31
		};

		constexpr unsigned int s_MultiplyDeBruijnBitPosition2[32] =
		{
			0,  1, 28,  2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17,  4, 8,
			31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18,  6, 11,  5, 10, 9
		};

		
		template<typename T>
		constexpr std::enable_if_t<sizeof(T) <= sizeof(UInt32), unsigned int> IntegralLog2(UInt32 number)
		{
			// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
			number |= number >> 1; // first round down to one less than a power of 2
			number |= number >> 2;
			number |= number >> 4;
			number |= number >> 8;
			number |= number >> 16;

			return s_MultiplyDeBruijnBitPosition[static_cast<UInt32>(number * 0x07C4ACDDU) >> 27];
		}

		template<typename T>
		constexpr std::enable_if_t<(sizeof(T) > sizeof(UInt32)), unsigned int> IntegralLog2(T number)
		{
			static_assert(sizeof(T) % sizeof(UInt32) == 0, "Assertion failed");

			// Masking and shifting bits to the right (to bring it back to 32 bits)

			// Call of the function with 32 bits number, if the result is non-null we have our answer
			for (int i = sizeof(T)-sizeof(UInt32); i >= 0; i -= sizeof(UInt32))
			{
				// The 32 bits mask on the part we are treating
				T mask = T(std::numeric_limits<UInt32>::max()) << i*8;
				T val = (number & mask) >> i*8; // Masking and shifting bits to the right (to bring it back to 32 bits)

				// Call of the function with 32 bits number, if the result is non-null we have our answer
				unsigned int log2 = IntegralLog2<UInt32>(static_cast<UInt32>(val));
				if (log2)
					return log2 + i*8;
			}

			return 0;
		}

		template<typename T>
		constexpr std::enable_if_t<sizeof(T) <= sizeof(UInt32), unsigned int> IntegralLog2Pot(T number)
		{
			// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
			return s_MultiplyDeBruijnBitPosition2[static_cast<UInt32>(number * 0x077CB531U) >> 27];
		}

		template<typename T>
		constexpr std::enable_if_t<(sizeof(T) > sizeof(UInt32)), unsigned int> IntegralLog2Pot(T number)
		{
			static_assert(sizeof(T) % sizeof(UInt32) == 0, "Assertion failed");

			// The algorithm for logarithm in base 2 only works with numbers greater than 32 bits
			// This code subdivides the biggest number into 32 bits ones
			for (int i = sizeof(T) - sizeof(UInt32); i >= 0; i -= sizeof(UInt32))
			{
				// The 32 bits mask on the part we are treating
				T mask = T(std::numeric_limits<UInt32>::max()) << i*8;
				UInt32 val = UInt32((number & mask) >> i*8); // Masking and shifting bits to the right (to bring it back to 32 bits)

				// Call of the function with 32 bits number, if the result is non-null we have our answer
				unsigned int log2 = IntegralLog2Pot<UInt32>(val);
				if (log2 || val == 1)
					return log2 + i*8;
			}

			return 0;
		}

		template<typename T> constexpr std::enable_if_t<std::is_floating_point<T>::value, bool> NumberEquals(T a, T b, T maxDifference)
		{
			if NAZARA_IS_RUNTIME_EVAL()
				return std::abs(a - b) <= maxDifference;
			else
			{
				if (b > a)
					std::swap(a, b);

				T diff = a - b;
				return diff <= maxDifference;
			}
		}

		template<typename T> constexpr std::enable_if_t<!std::is_signed<T>::value || (!std::is_integral<T>::value && !std::is_floating_point<T>::value), bool> NumberEquals(T a, T b, T maxDifference)
		{
			if (b > a)
				std::swap(a, b);

			T diff = a - b;
			return diff <= maxDifference;
		}

		template<typename T> constexpr std::enable_if_t<std::is_signed<T>::value&& std::is_integral<T>::value, bool> NumberEquals(T a, T b, T maxDifference)
		{
			if (b > a)
				std::swap(a, b);

			using UnsignedT = std::make_unsigned_t<T>;
			return static_cast<UnsignedT>(a) - static_cast<UnsignedT>(b) <= static_cast<UnsignedT>(maxDifference);
		}
	}

	/*!
	* \ingroup utils
	* \brief Approaches the objective, beginning with value and with increment
	* \return The nearest value of the objective you can get with the value and the increment for one step
	*
	* \param value Initial value
	* \param objective Target value
	* \param increment One step value
	*/
	template<typename T>
	constexpr inline T Approach(T value, T objective, T increment)
	{
		if (value < objective)
			return std::min(value + increment, objective);
		else if (value > objective)
			return std::max(value - increment, objective);
		else
			return value;
	}

	/*!
	* \ingroup utils
	* \brief Clamps value between min and max and returns the expected value
	* \return If value is not in the interval of min..max, value obtained is the nearest limit of this interval
	*
	* \param value Value to clamp
	* \param min Minimum of the interval
	* \param max Maximum of the interval
	*/
	template<typename T>
	constexpr T Clamp(T value, T min, T max)
	{
		return std::max(std::min(value, max), min);
	}

	/*!
	* \ingroup utils
	* \brief Sets the nth bit of a number to 0
	* \return The resulting number
	*
	* \param number Number
	* \param bit Bit index
	*
	* \remark bit must be between 0 and BitCount<T>()
	*/
	template<typename T>
	constexpr T ClearBit(T number, T bit)
	{
		if NAZARA_IS_RUNTIME_EVAL()
			assert((bit < BitCount<T>()) && "bit index out of range");

		return number &= ~(T(1) << bit);
	}

	/*!
	* \ingroup utils
	* \brief Gets number of bits set in the number
	* \return The number of bits set to 1
	*
	* \param value The value to count bits
	*/
	template<typename T>
	NAZARA_CONSTEXPR20 inline std::size_t CountBits(T value)
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
#if defined(NAZARA_COMPILER_MSVC) && (defined(NAZARA_ARCH_x86) || defined(NAZARA_ARCH_x86_64))
			if constexpr (std::is_same_v<std::make_unsigned_t<T>, unsigned __int64>)
			{
#ifdef NAZARA_ARCH_x86_64
				return __popcnt64(static_cast<unsigned __int64>(value));
#else
				return __popcnt(static_cast<unsigned int>(value >> 32)) +
					__popcnt(static_cast<unsigned int>(value));
#endif
			}
			else if constexpr (std::is_same_v<std::make_unsigned_t<T>, unsigned int>)
				return __popcnt(static_cast<unsigned int>(value));
			else
			{
				static_assert(sizeof(T) <= sizeof(unsigned short));
				return __popcnt16(static_cast<unsigned short>(value));
			}
#elif defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
			if constexpr (std::is_same_v<std::make_unsigned_t<T>, unsigned long long>)
				return __builtin_popcountll(static_cast<unsigned long long>(value));
			else if constexpr (std::is_same_v<std::make_unsigned_t<T>, unsigned long>)
				return __builtin_popcountl(static_cast<unsigned long>(value));
			else
			{
				static_assert(sizeof(T) <= sizeof(int));
				return __builtin_popcount(static_cast<unsigned int>(value));
			}
#endif
		}

		// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
		std::size_t count = 0;
		while (value)
		{
			value &= value - 1;
			count++;
		}

		return count;
	}

	/*!
	* \ingroup utils
	* \brief Converts degree to radian
	* \return The representation in radian of the angle in degree (0..2*pi)
	*
	* \param degrees Angle in degree (this is expected between 0..360)
	*/
	template<typename T>
	constexpr T DegreeToRadian(T degrees)
	{
		return degrees * (Pi<T> / T(180.0));
	}

	template<typename T>
	constexpr unsigned int FindFirstBit(T number)
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
#if defined(NAZARA_COMPILER_MSVC)
			if constexpr (sizeof(T) <= sizeof(unsigned long))
			{
				unsigned long index = 0;
				if (_BitScanForward(&index, number) != 0)
					return index + 1;
				else
					return 0;
			}
			else
			{
#if defined(NAZARA_ARCH_aarch64) || defined(NAZARA_ARCH_x86_64)
				unsigned long index = 0;
				if (_BitScanForward64(&index, number) != 0)
					return index + 1;
				else
					return 0;
#else
				unsigned long index = 0;
				if (_BitScanForward(&index, SafeCast<unsigned long>(number & 0xFFFFFFFF)) != 0)
					return index + 1;

				if (_BitScanForward(&index, SafeCast<unsigned long>(number >> 32)) != 0)
					return index + 33;
				else
					return 0;
#endif
			}
#elif defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
			if constexpr (std::is_same_v<std::make_signed_t<T>, long long>)
				return __builtin_ffsll(static_cast<long long>(number));
			else if constexpr (std::is_same_v<std::make_signed_t<T>, long>)
				return __builtin_ffsl(static_cast<long>(number));
			else
			{
				static_assert(sizeof(T) <= sizeof(int));
				return __builtin_ffs(static_cast<int>(number));
			}
#endif
		}

		return (number) ? IntegralLog2Pot(number & -number) + 1 : 0;
	}

	/*!
	* \ingroup utils
	* \brief Gets the nearest power of two for the number
	* \return First power of two containing the number
	*
	* \param number Number to get nearest power
	*/
	template<typename T>
	constexpr inline T GetNearestPowerOfTwo(T number)
	{
		T x = 1;
		while (x < number)
			x <<= 1; // We multiply by 2

		return x;
	}

	/*!
	* \ingroup math
	* \brief Gets the log in base 2 of integral number
	* \return Log of the number (floor)
	*
	* \param number To get log in base 2
	*
	* \remark If number is 0, 0 is returned
	*/
	template<typename T>
	constexpr unsigned int IntegralLog2(T number)
	{
		// Proxy needed to avoid an overload problem
		return Detail::IntegralLog2<T>(number);
	}

	/*!
	* \ingroup math
	* \brief Gets the log in base 2 of integral number, only works for power of two !
	* \return Log of the number
	*
	* \param pot To get log in base 2
	*
	* \remark Only works for power of two
	* \remark If number is 0, 0 is returned
	*/
	template<typename T>
	constexpr unsigned int IntegralLog2Pot(T pot)
	{
		return Detail::IntegralLog2Pot<T>(pot);
	}

	/*!
	* \ingroup math
	* \brief Gets the power of integrals
	* \return base^exponent for integral
	*
	* \param base Base of the exponentation
	* \param exponent Power for the base
	*/
	template<typename T>
	constexpr T IntegralPow(T base, unsigned int exponent)
	{
		T r = 1;
		for (unsigned int i = 0; i < exponent; ++i)
			r *= base;

		return r;
	}

	/*!
	* \ingroup math
	* \brief Interpolates the value to other one with a factor of interpolation
	* \return A new value which is the interpolation of two values
	*
	* \param from Initial value
	* \param to Target value
	* \param interpolation Factor of interpolation
	*
	* \see Lerp
	*/
	template<typename T, typename T2>
	constexpr T Lerp(const T& from, const T& to, const T2& interpolation)
	{
		return static_cast<T>(from + interpolation * (to - from));
	}

	/*!
	* \ingroup math
	* \brief Multiplies X and Y, then add Z
	* \return The result of X * Y + Z
	*
	* \param x is X
	* \param y is Y
	* \param z is Z
	*
	* \remark This function is meant to use a special faster instruction in CPU if possible
	*/
	template<typename T>
	constexpr T MultiplyAdd(T x, T y, T z)
	{
		return x * y + z;
	}

#ifdef FP_FAST_FMAF
	template<>
	constexpr float MultiplyAdd(float x, float y, float z)
	{
		return std::fmaf(x, y, z);
	}
#endif

#ifdef FP_FAST_FMA
	template<>
	constexpr double MultiplyAdd(double x, double y, double z)
	{
		return std::fma(x, y, z);
	}
#endif

#ifdef FP_FAST_FMAL
	template<>
	constexpr long double MultiplyAdd(long double x, long double y, long double z)
	{
		return std::fmal(x, y, z);
	}
#endif
	
	/*!
	* \ingroup math
	* \brief Checks whether two numbers are equal
	* \return true if they are equal within a certain epsilon
	*
	* \param a First value
	* \param b Second value
	*/
	template<typename T>
	constexpr inline bool NumberEquals(T a, T b)
	{
		return NumberEquals(a, b, std::numeric_limits<T>::epsilon());
	}

	/*!
	* \ingroup math
	* \brief Checks whether two numbers are equal
	* \return true if they are equal within the max difference
	*
	* \param a First value
	* \param b Second value
	* \param maxDifference Epsilon of comparison (expected to be positive)
	*/
	template<typename T>
	constexpr inline bool NumberEquals(T a, T b, T maxDifference)
	{
		return Detail::NumberEquals(a, b, maxDifference);
	}

	/*!
	* \ingroup math
	* \brief Converts radian to degree
	* \return The representation in degree of the angle in radian (0..360)
	*
	* \param radians Angle in radian (this is expected between 0..2*pi)
	*/
	template<typename T>
	constexpr T RadianToDegree(T radians)
	{
		return radians * (T(180.0) / Pi<T>);
	}

	/*!
	* \ingroup utils
	* \brief Sets the nth bit of a number to 1
	* \return The resulting number
	*
	* \param number Number
	* \param bit Bit index
	*
	* \remark bit must be between 0 and BitCount<T>()
	*/
	template<typename T>
	constexpr T SetBit(T number, T bit)
	{
		if NAZARA_IS_RUNTIME_EVAL()
			assert((bit >= 0 && bit < BitCount<T>()) && "bit index out of range");
		
		return number |= (T(1) << bit);
	}

	/*!
	* \ingroup utils
	* \brief Checks if a specific bit is set on a number
	* \return True if the bit is set, false otherwise
	*
	* \param number Number
	* \param bit Bit index
	*
	* \remark bit must be between 0 and BitCount<T>()
	*/
	template<typename T>
	constexpr bool TestBit(T number, T bit)
	{
		if NAZARA_IS_RUNTIME_EVAL()
			assert((bit >= 0 && bit < BitCount<T>()) && "bit index out of range");
		
		return number & (T(1) << bit);
	}

	/*!
	* \ingroup utils
	* \brief Toggle the nth bit of a number
	* \return The resulting number
	*
	* \param number Number
	* \param bit Bit index
	* 
	* \remark bit must be between 0 and BitCount<T>()
	*/
	template<typename T>
	constexpr T ToggleBit(T number, T bit)
	{
		if NAZARA_IS_RUNTIME_EVAL()
			assert((bit >= 0 && bit < BitCount<T>()) &&  "bit index out of range");

		return number ^= (T(1) << bit);
	}

	/*!
	* \ingroup utils
	* \brief Converts a std::filesystem::path to a UTF-8 encoded std::string
	* \return A utf-8 string containing the path
	*
	* \param path Path
	*/
	inline std::string PathToString(const std::filesystem::path& path)
	{
		return FromUtf8String(path.generic_u8string());
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
#if NAZARA_CPP_VER >= NAZARA_CPP20
		return std::filesystem::path(ToUtf8String(path));
#else
		return std::filesystem::u8path(path);
#endif
	}

	/*!
	* \ingroup utils
	* \brief Converts a std::u8string to a std::string
	* \return A utf-8 string stored inside a std::string object
	*
	* \param str A UTF-8 string encoded in a std::u8string if C++20 or std::string for lower version
	*/
#if NAZARA_CPP_VER >= NAZARA_CPP20 && !defined(NAZARA_PLATFORM_ANDROID)
	inline std::string FromUtf8String(const std::u8string& str)
	{
		return std::string(reinterpret_cast<const char*>(str.data()), str.size());
	}

	std::string_view FromUtf8String(std::u8string_view str)
	{
		return std::string_view(reinterpret_cast<const char*>(str.data()), str.size());
	}
	
	std::string_view FromUtf8String(const char8_t* str)
	{
		return FromUtf8String(std::u8string_view(str));
	}
#else
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
#if NAZARA_CPP_VER >= NAZARA_CPP20 && !defined(NAZARA_PLATFORM_ANDROID)
	inline std::u8string ToUtf8String(const std::string& str)
	{
		return std::u8string(reinterpret_cast<const char8_t*>(str.data()), str.size());
	}

	std::u8string_view ToUtf8String(std::string_view str)
	{
		return std::u8string_view(reinterpret_cast<const char8_t*>(str.data()), str.size());
	}
	
	std::u8string_view ToUtf8String(const char* str)
	{
		return ToUtf8String(std::string_view(str));
	}
#else
	inline std::string ToUtf8String(std::string str)
	{
		return str; // dummy
	}

	inline std::string_view ToUtf8String(std::string_view str)
	{
		return str; // dummy
	}
#endif

	/*!
	* \ingroup utils
	* \brief Access a non-typed struct field by offset
	* \return A pointer to the field of the asked type
	*
	* \param basePtr Pointer to the start of the struct
	* \param offset Offset to the field (as generated by offsetof or similar)
	*/
	template<typename T>
	decltype(auto) AccessByOffset(void* basePtr, std::size_t offset)
	{
		if constexpr (std::is_lvalue_reference_v<T>)
			return *reinterpret_cast<std::remove_reference_t<T>*>(static_cast<UInt8*>(basePtr) + offset);
		else if constexpr (std::is_pointer_v<T>)
			return reinterpret_cast<T>(static_cast<UInt8*>(basePtr) + offset);
		else
			static_assert(AlwaysFalse<T>(), "AccessByOffset requires a reference or pointer type");
	}

	/*!
	* \ingroup utils
	* \brief Access a non-typed struct field by offset
	* \return A pointer to the field of the asked type
	*
	* \param basePtr Pointer to the start of the struct
	* \param offset Offset to the field (as generated by offsetof or similar)
	*/
	template<typename T>
	decltype(auto) AccessByOffset(const void* basePtr, std::size_t offset)
	{
		static_assert(std::is_lvalue_reference_v<T> || std::is_pointer_v<T>);

		if constexpr (std::is_lvalue_reference_v<T>)
			return *reinterpret_cast<std::remove_reference_t<T>*>(static_cast<const UInt8*>(basePtr) + offset);
		else if constexpr (std::is_pointer_v<T>)
			return reinterpret_cast<T>(static_cast<const UInt8*>(basePtr) + offset);
		else
			static_assert(AlwaysFalse<T>(), "AccessByOffset requires a reference or pointer type");
	}

	/*!
	* \ingroup utils
	* \brief Align an offset
	* \return Aligned offset according to alignment
	*
	* \param offset Base offset
	* \param alignment Non-zero alignment
	*
	* \see AlignPow2
	*/
	template<typename T>
	constexpr T Align(T offset, T alignment)
	{
		if NAZARA_IS_RUNTIME_EVAL()
			assert(alignment > 0);

		return ((offset + alignment - 1) / alignment) * alignment;
	}

	/*!
	* \ingroup utils
	* \brief Align an offset
	* \return Aligned offset according to a power of two alignment
	*
	* \param offset Base offset
	* \param alignment Non-zero power of two alignment
	*
	* \see Align
	* \remark This function is quicker than Align but only works with power of two alignment values
	*/
	template<typename T>
	constexpr T AlignPow2(T offset, T alignment)
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert(alignment > 0);
			assert(IsPowerOfTwo(alignment));
		}

		return (offset + alignment - 1) & ~(alignment - 1);
	}

	/*!
	* \ingroup utils
	* \brief Returns the number of bits occupied by the type T
	* \return Number of bits occupied by the type
	*/
	template<typename T>
	constexpr std::size_t BitCount()
	{
		return CHAR_BIT * sizeof(T);
	}

	/*!
	* \ingroup utils
	* \brief Returns the number of elements in a C-array
	* \return The number of elements
	*
	* \see CountOf
	*/
	template<typename T, std::size_t N>
	constexpr std::size_t CountOf(T(&)[N]) noexcept
	{
		return N;
	}

	/*!
	* \ingroup utils
	* \brief Returns the number of elements in a container
	* \return The number of elements
	*
	* \param c Container with the member function "size()"
	*
	* \see CountOf
	*/
	template<typename T>
	std::size_t CountOf(const T& c)
	{
		return c.size();
	}

	/*!
	* \ingroup utils
	* \brief Check if a value is a power of two
	* \return true if value is a power of two
	*
	* \param value Non-zero value
	*/
	template<typename T>
	constexpr bool IsPowerOfTwo(T value)
	{
		if NAZARA_IS_RUNTIME_EVAL()
			assert(value != 0);
		
		return (value & (value - 1)) == 0;
	}

	/*!
	* \ingroup utils
	* \brief Helper function to retrieve a key in a map which has to exist
	* \return Value associated with key
	*
	* \param map Map
	* \param key Key, has to exist in map
	*/
	template<typename M, typename T>
	auto& Retrieve(M& map, const T& key)
	{
		auto it = map.find(key);
		assert(it != map.end());
		return it->second;
	}

	/*!
	* \ingroup utils
	* \brief Helper function to retrieve a key in a map which has to exist
	* \return Value associated with key
	*
	* \param map Map
	* \param key Key, has to exist in map
	*/
	template<typename M, typename T>
	const auto& Retrieve(const M& map, const T& key)
	{
		auto it = map.find(key);
		assert(it != map.end());
		return it->second;
	}

	/*!
	* \ingroup utils
	* \brief Reverse the bit order of the integer
	* \return integer with reversed bits
	*
	* \param integer Integer whose bits are to be reversed
	*/
	template<typename T>
	constexpr T ReverseBits(T integer)
	{
		T reversed = 0;
		for (std::size_t i = 0; i < sizeof(T); ++i)
			reversed |= T(Detail::BitReverseTable256[(integer >> i * 8) & 0xFF]) << (sizeof(T) * 8 - (i + 1) * 8);

		return reversed;
	}

	template<typename To, typename From>
	To SafeCast(From&& value)
	{
#ifdef NAZARA_COMPILER_MSVC
	// Disable unreachable code warnings
	#pragma warning(push)
	#pragma warning(disable: 4702)
#endif

#if defined(NAZARA_DEBUG) && !defined(NDEBUG)

		if constexpr (std::is_integral_v<To>)
		{
			if constexpr (std::is_enum_v<From>)
			{
				return SafeCast<To>(static_cast<std::underlying_type_t<From>>(value));
			}
			else if constexpr (std::is_floating_point_v<From>)
			{
				assert(std::floor(value) == value);

				assert(value <= static_cast<From>(std::numeric_limits<To>::max()));
				assert(value >= static_cast<From>(std::numeric_limits<To>::lowest()));
			}
			else if constexpr (std::is_integral_v<From>)
			{
				// Type capable of storing the biggest value between the two types
				using MaxValueType = std::conditional_t<((!std::is_signed_v<From> && std::is_signed_v<To>) || (std::is_signed_v<From> == std::is_signed_v<To> && sizeof(From) > sizeof(To))), From, To>;
				// Type capable of storing the smallest value between the two types
				using MinValueType = std::conditional_t<((std::is_signed_v<From> && !std::is_signed_v<To>) || (std::is_signed_v<From> == std::is_signed_v<To> && sizeof(From) > sizeof(To))), From, To>;

				if constexpr (!std::is_signed_v<To>)
					assert(value >= 0);

				assert(static_cast<MaxValueType>(value) <= static_cast<MaxValueType>(std::numeric_limits<To>::max()));
				assert(static_cast<MinValueType>(value) >= static_cast<MinValueType>(std::numeric_limits<To>::lowest()));
			}
		}
		else if constexpr (std::is_enum_v<To>)
		{
			return static_cast<To>(SafeCast<std::underlying_type_t<To>>(value));
		}
		else if constexpr (std::is_floating_point_v<To>)
		{
			if constexpr (std::is_floating_point_v<From>)
			{
				// Type capable of storing the biggest value between the two types
				using MaxValueType = std::conditional_t<(sizeof(From) > sizeof(To)), From, To>;
				// Type capable of storing the smallest value between the two types
				using MinValueType = std::conditional_t<(sizeof(From) > sizeof(To)), From, To>;

				assert(static_cast<MaxValueType>(value) <= static_cast<MaxValueType>(std::numeric_limits<To>::max()));
				assert(static_cast<MinValueType>(value) >= static_cast<MinValueType>(std::numeric_limits<To>::lowest()));
			}
		}
		else if constexpr (std::is_reference_v<To>)
		{
			if constexpr (std::is_reference_v<From>)
			{
				using BaseFromType = std::remove_reference_t<std::remove_cv_t<From>>;
				using BaseToType = std::remove_reference_t<std::remove_cv_t<To>>;

				if constexpr (!std::is_same_v<BaseFromType, BaseToType> && std::is_base_of_v<From, To> && std::is_polymorphic_v<From>)
				{
					using ToPtr = std::add_pointer_t<std::remove_reference_t<To>>;
					assert(dynamic_cast<ToPtr>(&value) != nullptr);
				}
			}
		}
		else if constexpr (std::is_pointer_v<To>)
		{
			if constexpr (std::is_pointer_v<From>)
			{
				using BaseFromType = std::remove_pointer_t<std::remove_cv_t<From>>;
				using BaseToType = std::remove_pointer_t<std::remove_cv_t<To>>;

				if constexpr (!std::is_same_v<BaseFromType, BaseToType> && std::is_base_of_v<From, To> && std::is_polymorphic_v<From>)
				{
					assert(dynamic_cast<To>(value) != nullptr);
				}
			}
		}

#endif

		return static_cast<To>(value);

#ifdef NAZARA_COMPILER_MSVC
	#pragma warning(pop)
#endif
	}

	template<typename T, typename U>
	std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& ptr)
	{
		return std::unique_ptr<T>(SafeCast<T*>(ptr.release()));
	}

	template<typename T>
	constexpr auto UnderlyingCast(T value) -> std::underlying_type_t<T>
	{
		return static_cast<std::underlying_type_t<T>>(value);
	}

	template<typename T> struct PointedType<T*>                { using type = T; };
	template<typename T> struct PointedType<T* const>          { using type = T; };
	template<typename T> struct PointedType<T* volatile>       { using type = T; };
	template<typename T> struct PointedType<T* const volatile> { using type = T; };
}

