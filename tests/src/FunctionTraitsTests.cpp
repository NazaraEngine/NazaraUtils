#include <NazaraUtils/FunctionTraits.hpp>

// This is a compilation test

static_assert(!Nz::IsFunctor_v<int>);

void Func(float, int, long long, short)
{

}

static_assert(Nz::IsFunctor_v<decltype(Func)>);
static_assert(Nz::FunctionTraits<decltype(Func)>::ArgCount == 4);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(Func)>::ArgType<2>, long long>);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(Func)>::FuncType, void(float, int, long long, short)>);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(Func)>::ReturnType, void>);

struct S {};

class C
{
public:
	void Method(const S&, double)
	{

	}

	void Method() const&
	{

	}
};

constexpr auto MethodWithoutParam = Nz::Overload<>(&C::Method);

static_assert(Nz::IsFunctor_v<decltype(MethodWithoutParam)>);
static_assert(Nz::FunctionTraits<decltype(MethodWithoutParam)>::ArgCount == 0);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(MethodWithoutParam)>::FuncType, void()>);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(MethodWithoutParam)>::ReturnType, void>);

constexpr auto MethodWithParam = Nz::Overload<const S&, double>(&C::Method);

static_assert(Nz::IsFunctor_v<decltype(MethodWithParam)>);
static_assert(Nz::FunctionTraits<decltype(MethodWithParam)>::ArgCount == 2);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(MethodWithParam)>::ArgType<0>, const S&>);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(MethodWithParam)>::FuncType, void(const S&, double)>);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(MethodWithParam)>::ReturnType, void>);

constexpr auto Lambda = [](double)
{
	return 0;
};

static_assert(Nz::IsFunctor_v<decltype(Lambda)>);
static_assert(Nz::FunctionTraits<decltype(Lambda)>::ArgCount == 1);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(Lambda)>::ArgType<0>, double>);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(Lambda)>::FuncType, int(double)>);
static_assert(std::is_same_v<Nz::FunctionTraits<decltype(Lambda)>::ReturnType, int>);

struct Functor
{
	long operator()(int a, int b)
	{
		return a + b;
	}
};

static_assert(Nz::IsFunctor_v<Functor>);
static_assert(Nz::FunctionTraits<Functor>::ArgCount == 2);
static_assert(std::is_same_v<Nz::FunctionTraits<Functor>::ArgType<0>, int>);
static_assert(std::is_same_v<Nz::FunctionTraits<Functor>::FuncType, long(int, int)>);
static_assert(std::is_same_v<Nz::FunctionTraits<Functor>::ReturnType, long>);
