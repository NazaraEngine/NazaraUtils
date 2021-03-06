// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_OFFSETOF_HPP
#define NAZARA_UTILS_OFFSETOF_HPP

// By "Jesse Good" from SO:
// http://stackoverflow.com/questions/12811330/c-compile-time-offsetof-inside-a-template?answertab=votes#tab-top

namespace Nz
{
	namespace Detail
	{
		template <typename T, typename M> T GetClassType(M T::*);
		template <typename T, typename M> M GetMemberType(M T::*);

		template <typename T, typename R, R T::*M>
		constexpr std::size_t OffsetOf()
		{
			return reinterpret_cast<std::size_t>(&((static_cast<T*>(0))->*M));
		}
	}
}

#define NazaraOffsetOf(type, member) Nz::Detail::OffsetOf<decltype(Nz::Detail::GetClassType(&type::member)), decltype(Nz::Detail::GetMemberType(&type::member)), &type::member>()

#endif // NAZARA_UTILS_OFFSETOF_HPP
