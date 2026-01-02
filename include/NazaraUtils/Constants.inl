// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/Constants.hpp>

namespace Nz
{
	namespace Detail
	{
#define NAZARA_MATH_CONSTANT(Name) \
		struct Name##Caster \
		{ \
			template<typename To> NAZARA_FORCEINLINE constexpr operator To() const { return Name<To>(); } \
		}
		
		NAZARA_MATH_CONSTANT(Infinity);
		NAZARA_MATH_CONSTANT(MaxValue);
		NAZARA_MATH_CONSTANT(MinValue);
		NAZARA_MATH_CONSTANT(NaN);
		NAZARA_MATH_CONSTANT(HalfPi);
		NAZARA_MATH_CONSTANT(Pi);
		NAZARA_MATH_CONSTANT(Sqrt2);
		NAZARA_MATH_CONSTANT(Sqrt3);
		NAZARA_MATH_CONSTANT(Sqrt5);
		NAZARA_MATH_CONSTANT(Tau);

#undef NAZARA_MATH_CONSTANT
	}

	template<>
	struct MathConstants<void>
	{
		static NAZARA_FORCEINLINE constexpr auto Infinity() { return Detail::InfinityCaster{}; }
		static NAZARA_FORCEINLINE constexpr auto Max()      { return Detail::MaxValueCaster{}; }
		static NAZARA_FORCEINLINE constexpr auto Min()      { return Detail::MinValueCaster{}; }
		static NAZARA_FORCEINLINE constexpr auto NaN()      { return Detail::NaNCaster{}; }
		static NAZARA_FORCEINLINE constexpr auto HalfPi()   { return Detail::HalfPiCaster{}; }
		static NAZARA_FORCEINLINE constexpr auto Pi()       { return Detail::PiCaster{}; }
		static NAZARA_FORCEINLINE constexpr auto Sqrt2()    { return Detail::Sqrt2Caster{}; }
		static NAZARA_FORCEINLINE constexpr auto Sqrt3()    { return Detail::Sqrt3Caster{}; }
		static NAZARA_FORCEINLINE constexpr auto Sqrt5()    { return Detail::Sqrt5Caster{}; }
		static NAZARA_FORCEINLINE constexpr auto Tau()      { return Detail::TauCaster{}; }
	};
}

