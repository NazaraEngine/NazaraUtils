// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/MathUtils.hpp>

namespace Nz
{
	namespace Detail
	{
		constexpr UInt8 s_BitReverseTable256[256] =
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
			static_assert(sizeof(T) % sizeof(UInt32) == 0);

			// Masking and shifting bits to the right (to bring it back to 32 bits)

			// Call of the function with 32 bits number, if the result is non-null we have our answer
			for (int i = sizeof(T) - sizeof(UInt32); i >= 0; i -= sizeof(UInt32))
			{
				// The 32 bits mask on the part we are treating
				T mask = T(std::numeric_limits<UInt32>::max()) << i * 8;
				T val = (number & mask) >> i * 8; // Masking and shifting bits to the right (to bring it back to 32 bits)

				// Call of the function with 32 bits number, if the result is non-null we have our answer
				unsigned int log2 = IntegralLog2<UInt32>(static_cast<UInt32>(val));
				if (log2)
					return log2 + i * 8;
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
				T mask = T(std::numeric_limits<UInt32>::max()) << i * 8;
				UInt32 val = UInt32((number & mask) >> i * 8); // Masking and shifting bits to the right (to bring it back to 32 bits)

				// Call of the function with 32 bits number, if the result is non-null we have our answer
				unsigned int log2 = IntegralLog2Pot<UInt32>(val);
				if (log2 || val == 1)
					return log2 + i * 8;
			}

			return 0;
		}

		template<typename T> constexpr std::enable_if_t<std::is_floating_point<T>::value, bool> NumberEquals(T a, T b, T maxDifference)
		{
			if NAZARA_IS_RUNTIME_EVAL()
			{
				return std::abs(a - b) <= maxDifference;
			}
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

	template<NAZARA_STD_CONCEPT_T(std::integral) T1, NAZARA_STD_CONCEPT_T(std::integral) T2>
	constexpr T1 ArithmeticRightShift(T1 value, T2 shift) noexcept
	{
		T1 result = 0; // Uninitialized values are forbidden in C++17 constexpr contexts
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20)
		// C++20 ensures that right shift performs an arthmetic shift on signed integers
		result = value >> shift;
#else
		// Implement arithmetic shift on C++ <=17
		if constexpr (std::is_signed_v<T2>)
		{
			if (value < 0 && shift > 0)
				result = (value >> shift) | ~(~static_cast<std::make_unsigned_t<T1>>(0u) >> shift);
			else
				result = value >> shift;
		}
		else
			result = value >> shift;
#endif

		return result;
	}

	namespace Detail
	{
		template<typename T, typename = void>
		struct ByteSwapImpl
		{
			static T Perform(T value)
			{
				// Generic byte swap
				SwapBytes(&value, sizeof(T));
				return value;
			}
		};

		template<typename T>
		struct ByteSwapImpl<T, std::enable_if_t<BitCount<T> == 8>>
		{
			static constexpr T Perform(T value)
			{
				return value;
			}
		};

		template<typename T>
		struct ByteSwapImpl<T, std::enable_if_t<BitCount<T> == 16 && std::is_integral_v<T>>>
		{
			static constexpr T Perform(T value)
			{
				if NAZARA_IS_RUNTIME_EVAL()
				{
#if defined(NAZARA_COMPILER_MSVC)
					return static_cast<T>(_byteswap_ushort(static_cast<unsigned short>(value)));
#elif defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
					return static_cast<T>(__builtin_bswap16(static_cast<uint16_t>(value)));
#endif
				}

				return static_cast<T>(((value & 0x00FF) << 8) |
					((value & 0xFF00) >> 8));
			}
		};

		template<typename T>
		struct ByteSwapImpl<T, std::enable_if_t<BitCount<T> == 32 && std::is_integral_v<T>>>
		{
			static constexpr T Perform(T value)
			{
				if NAZARA_IS_RUNTIME_EVAL()
				{
#if defined(NAZARA_COMPILER_MSVC)
					return static_cast<T>(_byteswap_ulong(static_cast<unsigned long>(value)));
#elif defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
					return static_cast<T>(__builtin_bswap32(static_cast<uint32_t>(value)));
#endif
				}

				return static_cast<T>(((value & 0x000000FF) << 24) |
					((value & 0x0000FF00) << 8) |
					((value & 0x00FF0000) >> 8) |
					((value & 0xFF000000) >> 24));
			}
		};

		template<typename T>
		struct ByteSwapImpl<T, std::enable_if_t<BitCount<T> == 64 && std::is_integral_v<T>>>
		{
			static constexpr T Perform(T value)
			{
				if NAZARA_IS_RUNTIME_EVAL()
				{
#if defined(NAZARA_COMPILER_MSVC)
					return static_cast<T>(_byteswap_uint64(static_cast<unsigned __int64>(value)));
#elif defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
					return static_cast<T>(__builtin_bswap64(static_cast<uint64_t>(value)));
#endif
				}

				return static_cast<T>(((value & 0x00000000000000FFULL) << 56) |
					((value & 0x000000000000FF00ULL) << 40) |
					((value & 0x0000000000FF0000ULL) << 24) |
					((value & 0x00000000FF000000ULL) << 8) |
					((value & 0x000000FF00000000ULL) >> 8) |
					((value & 0x0000FF0000000000ULL) >> 24) |
					((value & 0x00FF000000000000ULL) >> 40) |
					((value & 0xFF00000000000000ULL) >> 56));
			}
		};

		template<>
		struct ByteSwapImpl<float>
		{
			static NAZARA_CONSTEXPR_BITCAST float Perform(float value)
			{
				return BitCast<float>(Detail::ByteSwapImpl<UInt32>::Perform(BitCast<UInt32>(value)));
			}
		};

		template<>
		struct ByteSwapImpl<double>
		{
			static NAZARA_CONSTEXPR_BITCAST double Perform(double value)
			{
				return BitCast<double>(Detail::ByteSwapImpl<UInt64>::Perform(BitCast<UInt64>(value)));
			}
		};
	}

	template<NAZARA_STD_CONCEPT_T(Arithmetic) T>
	[[nodiscard]] constexpr T ByteSwap(T value) noexcept
	{
		return Detail::ByteSwapImpl<T>::Perform(value);
	}

	/*!
	* \ingroup utils
	* \brief Sets the nth bit of a number to 0
	* \return The resulting number
	*
	* \param number Number
	* \param bit Bit index
	*
	* \remark bit must be between 0 and BitCount<T>
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T ClearBit(T number, T bit) noexcept
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert((bit < BitCount<T>) && "bit index out of range");
		}

		return number &= ~(T(1) << bit);
	}

	/*!
	* \ingroup utils
	* \brief Gets number of bits set in the number
	* \return The number of bits set to 1
	*
	* \param value The value to count bits
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] NAZARA_CONSTEXPR20 std::size_t CountBits(T value) noexcept
	{
		static_assert(std::is_integral_v<T>);

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
			if constexpr (sizeof(T) == sizeof(unsigned long long))
				return __builtin_popcountll(static_cast<unsigned long long>(value));
			else if constexpr (sizeof(T) == sizeof(unsigned long))
				return __builtin_popcountl(static_cast<unsigned long>(value));
			else
			{
				static_assert(sizeof(T) <= sizeof(unsigned int));
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

	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] NAZARA_CONSTEXPR20 unsigned int FindFirstBit(T number) noexcept
	{
		static_assert(std::is_integral_v<T>);

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
			if constexpr (sizeof(T) == sizeof(long long))
				return __builtin_ffsll(static_cast<long long>(number));
			else if constexpr (sizeof(T) == sizeof(long))
				return __builtin_ffsl(static_cast<long>(number));
			else
			{
				static_assert(sizeof(T) <= sizeof(long));
				return __builtin_ffs(static_cast<int>(number));
			}
#endif
		}

NAZARA_WARNING_PUSH()
NAZARA_WARNING_MSVC_DISABLE(4146) // unary minus operator applied to unsigned type, result still unsigned

		return (number) ? IntegralLog2Pot(number & -number) + 1 : 0;

NAZARA_WARNING_POP()

	}

	/*!
	* \ingroup utils
	* \brief Reverse the bit order of the integer
	* \return integer with reversed bits
	*
	* \param integer Integer whose bits are to be reversed
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T ReverseBits(T integer) noexcept
	{
		T reversed = 0;
		for (std::size_t i = 0; i < sizeof(T); ++i)
			reversed |= T(Detail::s_BitReverseTable256[(integer >> i * 8) & 0xFF]) << (sizeof(T) * 8 - (i + 1) * 8);

		return reversed;
	}

	/*!
	* \ingroup utils
	* \brief Sets the nth bit of a number to 1
	* \return The resulting number
	*
	* \param number Number
	* \param bit Bit index
	*
	* \remark bit must be between 0 and BitCount<T>
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T SetBit(T number, T bit) noexcept
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert((bit >= 0 && bit < BitCount<T>) && "bit index out of range");
		}

		return number |= (T(1) << bit);
	}

	/*!
	* \ingroup utils
	* \brief Swaps the byte for endianness operations
	*
	* \param buffer Raw memory
	* \param size Size to change endianness
	*
	* \remark If size is greater than the preallocated buffer, the behavior is undefined
	*/
	inline void SwapBytes(void* buffer, std::size_t size) noexcept
	{
		UInt8* bytes = static_cast<UInt8*>(buffer);
		std::size_t i = 0;
		std::size_t j = size - 1;

		while (i < j)
			std::swap(bytes[i++], bytes[j--]);
	}

	/*!
	* \ingroup utils
	* \brief Checks if a specific bit is set on a number
	* \return True if the bit is set, false otherwise
	*
	* \param number Number
	* \param bit Bit index
	*
	* \remark bit must be between 0 and BitCount<T>
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr bool TestBit(T number, T bit) noexcept
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert((bit >= 0 && bit < BitCount<T>) && "bit index out of range");
		}

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
	* \remark bit must be between 0 and BitCount<T>
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T ToggleBit(T number, T bit) noexcept
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert((bit >= 0 && bit < BitCount<T>) && "bit index out of range");
		}

		return number ^= (T(1) << bit);
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
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T Align(T offset, T alignment) noexcept
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert(alignment > 0);
		}

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
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T AlignPow2(T offset, T alignment) noexcept
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert(alignment > 0);
			assert(IsPow2(alignment));
		}

		return (offset + alignment - 1) & ~(alignment - 1);
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
	[[nodiscard]] constexpr T Approach(T value, T objective, T increment) noexcept
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
	[[nodiscard]] constexpr T Clamp(T value, T min, T max) noexcept
	{
		return std::max(std::min(value, max), min);
	}

	/*!
	* \ingroup utils
	* \brief Converts degree to radian
	* \return The representation in radian of the angle in degree (0..2*pi)
	*
	* \param degrees Angle in degree (this is expected between 0..360)
	*/
	template<NAZARA_STD_CONCEPT_T(std::floating_point) T>
	[[nodiscard]] constexpr T DegreeToRadian(T degrees) noexcept
	{
		return degrees * (Pi<T>() / T(180.0));
	}

	/*!
	* \ingroup utils
	* \brief Gets the nearest power of two for the number
	* \return First power of two containing the number
	*
	* \param number Number to get nearest power
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T GetNearestPowerOfTwo(T number) noexcept
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
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr unsigned int IntegralLog2(T number) noexcept
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
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr unsigned int IntegralLog2Pot(T pot) noexcept
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
	[[nodiscard]] constexpr T IntegralPow(T base, unsigned int exponent) noexcept
	{
		T r = 1;
		for (unsigned int i = 0; i < exponent; ++i)
			r *= base;

		return r;
	}

	/*!
	* \ingroup math
	* \brief Tests if a floating-point is a finite value (with support for constexpr)
	* \return true if value is neither NaN or infinity
	*
	* \param value Floating-point value to test
	*/
	template<NAZARA_STD_CONCEPT_T(std::floating_point) T>
	[[nodiscard]] NAZARA_CONSTEXPR20 bool IsFinite(T value) noexcept
	{
		if NAZARA_IS_CONSTEVAL()
		{
			return value == value && value < Infinity<T>() && value > -Infinity<T>();
		}
		else
		{
			return std::isfinite(value);
		}
	}

	/*!
	* \ingroup math
	* \brief Tests if a floating-point is an infinite value (with support for constexpr)
	* \return true if value is positive or negative infinity
	*
	* \param value Floating-point value to test
	*/
	template<NAZARA_STD_CONCEPT_T(std::floating_point) T>
	[[nodiscard]] NAZARA_CONSTEXPR20 bool IsInfinity(T value) noexcept
	{
		if NAZARA_IS_CONSTEVAL()
		{
			return value == Infinity<T>() || value == -Infinity<T>();
		}
		else
		{
			return std::isinf(value);
		}
	}

	/*!
	* \ingroup math
	* \brief Tests if a floating-point is NaN (with support for constexpr)
	* \return true if value is NaN
	*
	* \param value Floating-point value to test
	*/
	template<NAZARA_STD_CONCEPT_T(std::floating_point) T>
	[[nodiscard]] NAZARA_CONSTEXPR20 bool IsNaN(T value) noexcept
	{
		if NAZARA_IS_CONSTEVAL()
		{
			return value != value;
		}
		else
		{
			return std::isnan(value);
		}
	}

	/*!
	* \ingroup utils
	* \brief Check if a value is a power of two
	* \return true if value is a power of two
	*
	* \param value Non-zero value
	*/
	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr bool IsPow2(T value) noexcept
	{
		if NAZARA_IS_RUNTIME_EVAL()
		{
			assert(value != 0);
		}

		return (value & (value - 1)) == 0;
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
	[[nodiscard]] constexpr T Lerp(const T& from, const T& to, const T2& interpolation) noexcept
	{
		return static_cast<T>(from + interpolation * (to - from));
	}

	template<typename T>
	[[nodiscard]] constexpr T Mod(T x, T y) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			if NAZARA_IS_RUNTIME_EVAL()
			{
				return std::fmod(x, y);
			}
			else
			{
				return x - static_cast<long long>(x / y) * y;
			}
		}
		else
			return x % y;
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
	[[nodiscard]] constexpr T MultiplyAdd(T x, T y, T z) noexcept
	{
		return x * y + z;
	}

#ifdef FP_FAST_FMAF
	template<>
	[[nodiscard]] constexpr float MultiplyAdd(float x, float y, float z) noexcept
	{
		return std::fmaf(x, y, z);
	}
#endif

#ifdef FP_FAST_FMA
	template<>
	[[nodiscard]] constexpr double MultiplyAdd(double x, double y, double z) noexcept
	{
		return std::fma(x, y, z);
	}
#endif

#ifdef FP_FAST_FMAL
	template<>
	[[nodiscard]] constexpr long double MultiplyAdd(long double x, long double y, long double z) noexcept
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
	[[nodiscard]] constexpr bool NumberEquals(T a, T b) noexcept
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
	[[nodiscard]] constexpr bool NumberEquals(T a, T b, T maxDifference) noexcept
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
	template<NAZARA_STD_CONCEPT_T(std::floating_point) T>
	[[nodiscard]] constexpr T RadianToDegree(T radians) noexcept
	{
		return radians * (T(180.0) / Pi<T>());
	}

	template<NAZARA_STD_CONCEPT_T(std::integral) T>
	[[nodiscard]] constexpr T RoundToPow2(T value) noexcept
	{
		// https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
		value--;

		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		if constexpr (BitCount<T> > 8)
			value |= value >> 8;
		if constexpr (BitCount<T> > 16)
			value |= value >> 16;
		if constexpr (BitCount<T> > 32)
			value |= value >> 32;

		value++;

		return value;
	}
}

