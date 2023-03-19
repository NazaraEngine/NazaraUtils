#include <NazaraUtils/TypeList.hpp>
#include <variant>
#include <type_traits>

// This is a compilation test

using T1 = Nz::TypeList<>;

static_assert(Nz::TypeListEmpty<T1>);
static_assert(Nz::TypeListSize<T1> == 0);

using T2 = Nz::TypeListAppend<T1, int>;

static_assert(!Nz::TypeListEmpty<T2>);
static_assert(Nz::TypeListSize<T2> == 1);
static_assert(std::is_same_v<T2, Nz::TypeList<int>>);

using T3 = Nz::TypeListPrepend<T2, float>;

static_assert(std::is_same_v<T3, Nz::TypeList<float, int>>);

using T4 = Nz::TypeListAppend<T3, float>;

static_assert(std::is_same_v<T4, Nz::TypeList<float, int, float>>);
static_assert(std::is_same_v<Nz::TypeListAt<T4, 0>, float>);
static_assert(std::is_same_v<Nz::TypeListAt<T4, 1>, int>);
static_assert(std::is_same_v<Nz::TypeListAt<T4, 2>, float>);

using T5 = Nz::TypeListUnique<T4>;

static_assert(std::is_same_v<T5, Nz::TypeList<float, int>>);

using T6 = Nz::TypeList<long long, short>;
using T7 = Nz::TypeListConcat<T5, T6>;

static_assert(std::is_same_v<T7, Nz::TypeList<float, int, long long, short>>);
static_assert(Nz::TypeListHas<T7, long long>);
static_assert(Nz::TypeListFind<T7, long long> == 2);
static_assert(!Nz::TypeListHas<T7, double>);
static_assert(Nz::TypeListFind<T7, double> == std::numeric_limits<std::size_t>::max());
static_assert(Nz::TypeListSize<T7> == 4);

using T7_Variant = Nz::TypeListInstantiate<T7, std::variant>;

static_assert(std::is_same_v<T7_Variant, std::variant<float, int, long long, short>>);

using T8 = Nz::TypeListTransform<T7, std::add_pointer>;

static_assert(std::is_same_v<T8, Nz::TypeList<float*, int*, long long*, short*>>);

using T9 = Nz::TypeListTransform<T8, std::remove_pointer>;

static_assert(std::is_same_v<T7, T9>);