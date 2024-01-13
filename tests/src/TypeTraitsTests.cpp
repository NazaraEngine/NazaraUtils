#include <NazaraUtils/TypeTraits.hpp>

// This is a compilation test

int foo(double, float);

static_assert(std::is_same_v<Nz::FunctionPtr<int(double, float)>, decltype(&foo)>);