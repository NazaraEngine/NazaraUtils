#include <NazaraUtils/TypeName.hpp>
#include <catch2/catch_test_macros.hpp>

class Foo {};
template<typename T> struct Class {};
template<int N> struct Int {};

static_assert(Nz::TypeName<int>() == "int");
#ifdef NAZARA_COMPILER_MSVC
static_assert(Nz::TypeName<Foo>() == "class Foo");
static_assert(Nz::TypeName<Class<Foo>>() == "struct Class<class Foo>");
static_assert(Nz::TypeName<Int<42>>() == "struct Int<42>");
#else
static_assert(Nz::TypeName<Foo>() == "Foo");
static_assert(Nz::TypeName<Class<Foo>>() == "Class<Foo>");
static_assert(Nz::TypeName<Int<42>>() == "Int<42>");
#endif

TEST_CASE("Type names", "[TypeHash]")
{
	// This could be checked by static_assert, but runtime checks allows to see the difference between expect and result
	CHECK(Nz::TypeName<int>() == "int");
#ifdef NAZARA_COMPILER_MSVC
	CHECK(Nz::TypeName<Foo>() == "class Foo");
	CHECK(Nz::TypeName<Class<Foo>>() == "struct Class<class Foo>");
	CHECK(Nz::TypeName<Int<42>>() == "struct Int<42>");
#else
	CHECK(Nz::TypeName<Foo>() == "Foo");
	CHECK(Nz::TypeName<Class<Foo>>() == "Class<Foo>");
	CHECK(Nz::TypeName<Int<42>>() == "Int<42>");
#endif
}
