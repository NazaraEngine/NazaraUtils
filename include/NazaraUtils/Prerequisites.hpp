/*
	Nazara Utility Library

	Copyright (C) 2015 Jérôme "Lynix" Leclercq (Lynix680@gmail.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifndef NAZARA_PREREQUISITES_HPP
#define NAZARA_PREREQUISITES_HPP

// Try to identify the compiler
#if defined(__BORLANDC__)
	#define NAZARA_COMPILER_BORDLAND
	#define NAZARA_DEPRECATED(txt)
	#define NAZARA_FUNCTION __FUNC__
#elif defined(__clang__)
	#define NAZARA_COMPILER_CLANG
	#define NAZARA_DEPRECATED(txt) __attribute__((__deprecated__(txt)))
	#define NAZARA_FUNCTION __PRETTY_FUNCTION__

	#ifdef __MINGW32__
		#define NAZARA_COMPILER_MINGW
		#ifdef __MINGW64_VERSION_MAJOR
			#define NAZARA_COMPILER_MINGW_W64
		#endif
	#endif
#elif defined(__GNUC__) || defined(__MINGW32__)
	#define NAZARA_COMPILER_GCC
	#define NAZARA_DEPRECATED(txt) __attribute__((__deprecated__(txt)))
	#define NAZARA_FUNCTION __PRETTY_FUNCTION__

	#ifdef __MINGW32__
		#define NAZARA_COMPILER_MINGW
		#ifdef __MINGW64_VERSION_MAJOR
			#define NAZARA_COMPILER_MINGW_W64
		#endif
	#endif
#elif defined(__INTEL_COMPILER) || defined(__ICL)
	#define NAZARA_COMPILER_INTEL
	#define NAZARA_DEPRECATED(txt)
	#define NAZARA_FUNCTION __FUNCTION__
#elif defined(_MSC_VER)
	#define NAZARA_COMPILER_MSVC
	#define NAZARA_DEPRECATED(txt) __declspec(deprecated(txt))
	#define NAZARA_FUNCTION __FUNCSIG__

	// __cplusplus isn't respected on MSVC without /Zc:__cplusplus flag
	#define NAZARA_CPP_VER _MSVC_LANG

	#pragma warning(disable: 4251)
#else
	#define NAZARA_COMPILER_UNKNOWN
	#define NAZARA_DEPRECATED(txt)
	#define NAZARA_FUNCTION __func__ // __func__ has been standardized in C++ 2011

	#pragma message This compiler is not fully supported
#endif

#ifndef NAZARA_CPP_VER
#define NAZARA_CPP_VER __cplusplus
#endif

#define NAZARA_CPP98 199711L
#define NAZARA_CPP11 201103L
#define NAZARA_CPP14 201402L
#define NAZARA_CPP17 201703L
#define NAZARA_CPP20 202002L

#if NAZARA_CPP_VER < NAZARA_CPP17
	#error Nazara requires C++17 or higher
#endif

// Try to identify target platform via defines
#if defined(_WIN32)
	#define NAZARA_PLATFORM_DESKTOP
	#define NAZARA_PLATFORM_WINDOWS

	#define NAZARA_EXPORT __declspec(dllexport)
	#define NAZARA_IMPORT __declspec(dllimport)

	// Somes defines for windows.h include..
	#if defined(NAZARA_BUILD)
		#ifndef WIN32_LEAN_AND_MEAN
			#define WIN32_LEAN_AND_MEAN
		#endif

		#ifndef NOMINMAX
			#define NOMINMAX
		#endif

		#if NAZARA_UTILS_WINDOWS_NT6
			#define NAZARA_WINNT 0x0600
		#else
			#define NAZARA_WINNT 0x0501
		#endif

		// Keep the actual define if existing and greater than our requirement
		#if defined(_WIN32_WINNT)
			#if _WIN32_WINNT < NAZARA_WINNT
				#undef _WIN32_WINNT
				#define _WIN32_WINNT NAZARA_WINNT
			#endif
		#else
			#define _WIN32_WINNT NAZARA_WINNT
		#endif
	#endif
#elif defined(__ANDROID__)
	#define NAZARA_PLATFORM_MOBILE
	#define NAZARA_PLATFORM_ANDROID
	#define NAZARA_PLATFORM_POSIX

	#define NAZARA_EXPORT __attribute__((visibility("default")))
	#define NAZARA_IMPORT __attribute__((visibility("default")))
#elif defined(__linux__)
	#define NAZARA_PLATFORM_DESKTOP
	#define NAZARA_PLATFORM_LINUX
	#define NAZARA_PLATFORM_POSIX

	#define NAZARA_EXPORT __attribute__((visibility("default")))
	#define NAZARA_IMPORT __attribute__((visibility("default")))
#elif defined(__FreeBSD__)
	#define NAZARA_PLATFORM_BSD
	#define NAZARA_PLATFORM_FREEBSD
	#define NAZARA_PLATFORM_POSIX

	#define NAZARA_EXPORT __attribute__((visibility("default")))
	#define NAZARA_IMPORT __attribute__((visibility("default")))
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE
		#define NAZARA_PLATFORM_MOBILE
		#define NAZARA_PLATFORM_IOS
	#else
		#define NAZARA_PLATFORM_DESKTOP
		#define NAZARA_PLATFORM_MACOS
	#endif
	#define NAZARA_PLATFORM_BSD
	#define NAZARA_PLATFORM_POSIX

	#define NAZARA_EXPORT __attribute__((visibility("default")))
	#define NAZARA_IMPORT __attribute__((visibility("default")))
#elif defined(__EMSCRIPTEN__)
	#define NAZARA_PLATFORM_WEB
	#define NAZARA_PLATFORM_POSIX

	#define NAZARA_EXPORT __attribute__((visibility("default")))
	#define NAZARA_IMPORT __attribute__((visibility("default")))
#else
	#pragma message "This operating system is not fully supported by the Nazara Engine"

	#define NAZARA_PLATFORM_UNKNOWN
	#define NAZARA_EXPORT
	#define NAZARA_IMPORT
#endif

// Likely/unlikely attributes
#ifndef NAZARA_NO_LIKELY_ATTRIBUTE

#if NAZARA_CPP_VER >= NAZARA_CPP20 || (defined(__has_cpp_attribute) && __has_cpp_attribute(likely))
	#define NAZARA_LIKELY(expr) (expr) [[likely]]
#endif

#if NAZARA_CPP_VER >= NAZARA_CPP20 || (defined(__has_cpp_attribute) && __has_cpp_attribute(unlikely))
	#define NAZARA_UNLIKELY(expr) (expr) [[unlikely]]
#endif

#if defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC) || defined(NAZARA_COMPILER_INTEL)

	#ifndef NAZARA_LIKELY
		#define NAZARA_LIKELY(expr) (__builtin_expect(!!(expr), 1))
	#endif

	#ifndef NAZARA_UNLIKELY
		#define NAZARA_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
	#endif

#endif

#endif // NAZARA_NO_LIKELY_ATTRIBUTE

#ifndef NAZARA_LIKELY
	#define NAZARA_LIKELY(expr) (expr)
#endif

#ifndef NAZARA_UNLIKELY
	#define NAZARA_UNLIKELY(expr) (expr)
#endif

// Detect arch
#ifndef NAZARA_NO_ARCH_DETECTION

	#if defined(__arm__) || defined(__thumb__) || defined(__ARM_ARCH_7__) || defined(_M_ARM)
		#define NAZARA_ARCH_arm
	#endif

	#if defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
		#define NAZARA_ARCH_aarch64
	#endif

	#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64) || defined (_M_X64)
		#define NAZARA_ARCH_x86_64
	#endif

	#if defined(__i386__) || defined(_M_IX86) || defined(_X86_)
		#define NAZARA_ARCH_x86
	#endif

	#if defined(__EMSCRIPTEN__)
		#define NAZARA_ARCH_wasm32
	#endif

	#if defined(NAZARA_ARCH_arm) \
	  + defined(NAZARA_ARCH_aarch64) \
	  + defined(NAZARA_ARCH_x86_64) \
	  + defined(NAZARA_ARCH_x86) \
	  + defined(NAZARA_ARCH_wasm32) != 1

	#error No or multiple arch detected! Please open an issue with details about your target system. You can define NAZARA_NO_ARCH_DETECTION to bypass this error.

	#endif

#endif // NAZARA_NO_ARCH_DETECTION


#ifdef NAZARA_UNITY_BUILD
	#define NAZARA_ANONYMOUS_NAMESPACE NAZARA_UNITY_ID
	#define NAZARA_USE_ANONYMOUS_NAMESPACE using namespace NAZARA_UNITY_ID;
	#define NAZARA_ANONYMOUS_NAMESPACE_PREFIX(a) NAZARA_UNITY_ID::a
#else
	#define NAZARA_ANONYMOUS_NAMESPACE
	#define NAZARA_USE_ANONYMOUS_NAMESPACE
	#define NAZARA_ANONYMOUS_NAMESPACE_PREFIX(a) a
#endif

// A bunch of useful macros
#define NazaraPrefix(a, prefix) prefix ## a
#define NazaraPrefixMacro(a, prefix) NazaraPrefix(a, prefix)
#define NazaraSuffix(a, suffix) a ## suffix
#define NazaraSuffixMacro(a, suffix) NazaraSuffix(a, suffix)
#define NazaraStringify(s) #s
#define NazaraStringifyMacro(s) NazaraStringify(s) // http://gcc.gnu.org/onlinedocs/cpp/Stringification.html#Stringification
#define NazaraUnused(a) (void) a

#include <climits>
#include <cstdint>

static_assert(CHAR_BIT == 8, "CHAR_BIT is expected to be 8");

static_assert(sizeof(int8_t)  == 1, "int8_t is not of the correct size" );
static_assert(sizeof(int16_t) == 2, "int16_t is not of the correct size");
static_assert(sizeof(int32_t) == 4, "int32_t is not of the correct size");
static_assert(sizeof(int64_t) == 8, "int64_t is not of the correct size");

static_assert(sizeof(uint8_t)  == 1, "uint8_t is not of the correct size" );
static_assert(sizeof(uint16_t) == 2, "uint16_t is not of the correct size");
static_assert(sizeof(uint32_t) == 4, "uint32_t is not of the correct size");
static_assert(sizeof(uint64_t) == 8, "uint64_t is not of the correct size");

namespace Nz
{
	using Int8 = int8_t;
	using UInt8 = uint8_t;

	using Int16 = int16_t;
	using UInt16 = uint16_t;

	using Int32 = int32_t;
	using UInt32 = uint32_t;

	using Int64 = int64_t;
	using UInt64 = uint64_t;
}

#endif // NAZARA_PREREQUISITES_HPP
