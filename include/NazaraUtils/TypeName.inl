// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

namespace Nz
{
	template<typename T>
	constexpr std::string_view TypeName()
	{
		constexpr std::string_view prettyFuncName = NAZARA_PRETTY_FUNCTION;

#ifdef NAZARA_COMPILER_MSVC
		constexpr std::string_view prefix = "Nz::TypeName<";
		constexpr std::string_view suffixes = ">";
#else
		constexpr std::string_view prefix = "T = ";
		constexpr std::string_view suffixes = ";]";
#endif

		constexpr std::size_t b = prettyFuncName.find(prefix);
		static_assert(b != prettyFuncName.npos);

		constexpr std::size_t e = prettyFuncName.substr(b).find_last_of(suffixes, b) + b;
		static_assert(e != prettyFuncName.npos);

		std::size_t p = b + prefix.size();

		std::string_view typeName = prettyFuncName.substr(p, e - p);

#ifdef NAZARA_COMPILER_MSVC
		// strip first-level class/struct in front of the type

		for (std::string_view prefix : { "class ", "struct ", "enum ", "enum class " })
		{
			if (typeName.compare(0, prefix.size(), prefix.data()) == 0)
			{
				typeName.remove_prefix(prefix.size());
				break;
			}
		}
#endif

		return typeName;
	}
}
