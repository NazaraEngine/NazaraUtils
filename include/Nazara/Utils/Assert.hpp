// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_ASSERT_HPP
#define NAZARA_UTILS_ASSERT_HPP

#include <Nazara/Prerequisites.hpp>

#ifndef NAZARA_UTILS_ASSERTION_FAILURE

#include <cstdlib>
#include <iostream>

#define NAZARA_UTILS_ASSERTION_FAILURE(err) { std::cerr << err << std::endl; std::abort(); }

#endif

#if NAZARA_UTILS_ENABLE_ASSERTS || defined(NAZARA_DEBUG)
	#define NazaraAssert(a, err) if (!(a)) NAZARA_UTILS_ASSERTION_FAILURE(err)
#else
	#define NazaraAssert(a, err) for (;;) break
#endif

#endif // NAZARA_UTILS_ASSERT_HPP
