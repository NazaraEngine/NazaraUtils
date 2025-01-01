// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

// Put the implementation in the global scope to avoid compilers (like GCC) stripping the namespace (Nz::TypeName<Nz::Type>() returning "Type" instead of "Nz::Type")
template<typename T>
constexpr std::string_view NzDetailTypeName()
{
	using namespace std::string_view_literals;

	constexpr std::string_view prettyFuncName = NAZARA_PRETTY_FUNCTION;

#ifdef NAZARA_COMPILER_MSVC
	constexpr std::string_view prefix = "NzDetailTypeName<"sv;
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

	for (std::string_view typePrefix : { "class "sv, "struct "sv, "enum "sv, "enum class "sv, "enum struct"sv })
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

namespace Nz
{
	template<typename T>
	constexpr std::string_view TypeName()
	{
		return NzDetailTypeName<T>();
	}
}
