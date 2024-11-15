#include <NazaraUtils/TypeName.hpp>
#include <catch2/catch_test_macros.hpp>

class Foo {};
template<typename T> struct Class {};
template<int N> struct Int {};

namespace Name
{
	enum Space {};
	enum class SpaceClass;
	enum struct SpaceStruct;
}

static_assert(Nz::TypeName<int>() == "int");
static_assert(Nz::TypeName<Foo>() == "Foo");
static_assert(Nz::TypeName<Int<42>>() == "Int<42>");
static_assert(Nz::TypeName<Name::Space>() == "Name::Space");
static_assert(Nz::TypeName<Name::SpaceClass>() == "Name::SpaceClass");
static_assert(Nz::TypeName<Name::SpaceStruct>() == "Name::SpaceStruct");
#ifdef NAZARA_COMPILER_MSVC
static_assert(Nz::TypeName<Class<Foo>>() == "Class<class Foo>");
#else
static_assert(Nz::TypeName<Class<Foo>>() == "Class<Foo>");
#endif

namespace Nz
{
	struct Test {};
	static_assert(TypeName<Test>() == "Nz::Test");
}

TEST_CASE("Type names", "[TypeHash]")
{
	// This could be checked by static_assert, but runtime checks allows to see the difference between expect and result
	CHECK(Nz::TypeName<int>() == "int");
	CHECK(Nz::TypeName<Foo>() == "Foo");
	CHECK(Nz::TypeName<Int<42>>() == "Int<42>");
	CHECK(Nz::TypeName<Name::Space>() == "Name::Space");
	CHECK(Nz::TypeName<Name::SpaceClass>() == "Name::SpaceClass");
	CHECK(Nz::TypeName<Name::SpaceStruct>() == "Name::SpaceStruct");
#ifdef NAZARA_COMPILER_MSVC
	CHECK(Nz::TypeName<Class<Foo>>() == "Class<class Foo>");
#else
	CHECK(Nz::TypeName<Class<Foo>>() == "Class<Foo>");
#endif
}
