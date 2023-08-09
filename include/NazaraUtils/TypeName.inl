// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

namespace Nz
{
	template<typename T>
	constexpr std::string_view TypeName()
	{
		using namespace std::string_view_literals;

		constexpr std::string_view prettyFuncName = NAZARA_PRETTY_FUNCTION;

#ifdef NAZARA_COMPILER_MSVC
		constexpr std::string_view prefix = "Nz::TypeName<"sv;
		constexpr std::string_view suffixes = ">"sv;
#else
		constexpr std::string_view prefix = "T = "sv;
		constexpr std::string_view suffixes = ";]"sv;
#endif

		constexpr std::size_t b = prettyFuncName.find(prefix);
		static_assert(b != prettyFuncName.npos);

		constexpr std::size_t e = prettyFuncName.substr(b).find_last_of(suffixes, b) + b;
		static_assert(e != prettyFuncName.npos);

		std::size_t p = b + prefix.size();

		std::string_view typeName = prettyFuncName.substr(p, e - p);

#ifdef NAZARA_COMPILER_MSVC
		// strip first-level class/struct in front of the type

		for (std::string_view typePrefix : { "class "sv, "struct "sv, "enum "sv, "enum class "sv })
		{
			if (typeName.substr(0, typePrefix.size()) == typePrefix)
			{
				typeName.remove_prefix(typePrefix.size());
				break;
			}
		}
#endif

		return typeName;
	}
}
