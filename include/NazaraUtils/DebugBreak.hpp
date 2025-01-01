// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_DEBUGBREAK_HPP
#define NAZARAUTILS_DEBUGBREAK_HPP

#include <NazaraUtils/Prerequisites.hpp>

// from https://github.com/nemequ/portable-snippets/blob/master/debug-trap/debug-trap.h
#if defined(__has_builtin) && !defined(__ibmxl__)
#  if __has_builtin(__builtin_debugtrap)
#    define NazaraDebugBreak() __builtin_debugtrap()
#  elif __has_builtin(__debugbreak)
#    define NazaraDebugBreak() __debugbreak()
#  endif
#endif

#if !defined(NazaraDebugBreak)
#  if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#    define NazaraDebugBreak() __debugbreak()
#  elif defined(__ARMCC_VERSION)
#    define NazaraDebugBreak() __breakpoint(42)
#  elif defined(__ibmxl__) || defined(__xlC__)
#    include <builtins.h>
#    define NazaraDebugBreak() __trap(42)
#  elif defined(__DMC__) && defined(_M_IX86)
    static inline void NazaraDebugBreak() { __asm int 3h; }
#  elif defined(__i386__) || defined(__x86_64__)
    static inline void NazaraDebugBreak() { __asm__ __volatile__("int3"); }
#  elif defined(__thumb__)
    static inline void NazaraDebugBreak() { __asm__ __volatile__(".inst 0xde01"); }
#  elif defined(__aarch64__)
    static inline void NazaraDebugBreak() { __asm__ __volatile__(".inst 0xd4200000"); }
#  elif defined(__arm__)
    static inline void NazaraDebugBreak() { __asm__ __volatile__(".inst 0xe7f001f0"); }
#  elif defined (__alpha__) && !defined(__osf__)
    static inline void NazaraDebugBreak() { __asm__ __volatile__("bpt"); }
#  elif defined(_54_)
    static inline void NazaraDebugBreak() { __asm__ __volatile__("ESTOP"); }
#  elif defined(_55_)
    static inline void NazaraDebugBreak() { __asm__ __volatile__(";\n .if (.MNEMONIC)\n ESTOP_1\n .else\n ESTOP_1()\n .endif\n NOP"); }
#  elif defined(_64P_)
    static inline void NazaraDebugBreak() { __asm__ __volatile__("SWBP 0"); }
#  elif defined(_6x_)
    static inline void NazaraDebugBreak() { __asm__ __volatile__("NOP\n .word 0x10000000"); }
#  elif defined(__STDC_HOSTED__) && (__STDC_HOSTED__ == 0) && defined(__GNUC__)
#    define NazaraDebugBreak() __builtin_trap()
#  else
#    include <signal.h>
#    if defined(SIGTRAP)
#      define NazaraDebugBreak() raise(SIGTRAP)
#    else
#      define NazaraDebugBreak() raise(SIGABRT)
#    endif
#  endif
#endif

#endif // NAZARAUTILS_DEBUGBREAK_HPP
