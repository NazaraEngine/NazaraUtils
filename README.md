[![Build and test](https://github.com/NazaraEngine/NazaraUtils/actions/workflows/build.yml/badge.svg)](https://github.com/NazaraEngine/NazaraUtils/actions/workflows/build.yml)

# Nazara Utility Library

Header-only utility library made for Nazara projects (mainly [Nazara Shading Language](https://github.com/NazaraEngine/ShaderLang) and [Nazara Engine](https://github.com/NazaraEngine/ShaderLang)), however it's open to anyone who wants to use it (or a portion of it) in its project.

Features:
- Portable (has been tested on Windows/Linux/macOS/Android/iOS/wasm with MSVC, Clang and GCC/MinGW)
- Header-only
- Platform and compiler detection
- Fully-featured bitsets
- Compile-time endianness detection
- EnumArray ("map" between an enum as a key and anything as a value)
- FixedVector (middleground between std::array and std::vector, std::vector but with a template capacity)
- Flags support (turn every enum into flags with operator overloading support)
- FunctionRef (lightweight references to functors, avoids std::function heap allocation for callbacks)
- Function traits
- Hashes (constexpr CRC32/FNV1a32/FNV1a64)
- A simple memory pool
- Result class (similar to Rust Result)
- Signals and slots
- Sparse pointers
- Stack-allocated arrays and vectors (with a runtime size/capacity)
- Metaprogramming type lists
- Type name extraction
- Intrinsics-powered constexpr utilities (BitCast, ByteSwap, CountBits, FindFirstBit)
- Some helpers (RAII capsule, automatic pointer and value movement wrappers)

## Authors

Jérôme "SirLynix" Leclercq - main developper (<lynix680@gmail.com>)   

## Links

[Discord](https://discord.gg/MvwNx73)  
