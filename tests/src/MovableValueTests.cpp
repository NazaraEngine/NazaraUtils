#include <NazaraUtils/MovableValue.hpp>
#include <CopyCounter.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
	struct Buffer
	{
		Buffer() = default;
		Buffer(int index) : bufferIndex(index) {}
		Buffer(const Buffer&) = default;
		Buffer(Buffer&&) noexcept = default;

		Buffer& operator=(const Buffer&) = default;
		Buffer& operator=(Buffer&&) noexcept = default;

		Nz::MovableLiteral<int, -1> bufferIndex;
	};
}

SCENARIO("MovableLiteral", "[MovableValue]")
{
	WHEN("testing constructors")
	{
		Buffer defaultBuffer;
		CHECK(defaultBuffer.bufferIndex == -1);

		Buffer initializedBuffer(1);
		CHECK(initializedBuffer.bufferIndex == 1);
	}

	WHEN("Testing copy constructor")
	{
		Buffer foo(42);
		Buffer bar(foo);

		CHECK(foo.bufferIndex == 42);
		CHECK(bar.bufferIndex == 42);
	}

	WHEN("Testing move constructor")
	{
		Buffer foo(42);
		Buffer bar(std::move(foo));

		CHECK(foo.bufferIndex == -1);
		CHECK(bar.bufferIndex == 42);
	}

	WHEN("Testing copy assignation")
	{
		Buffer foo(42);
		Buffer bar;

		bar = foo;

		CHECK(foo.bufferIndex == 42);
		CHECK(bar.bufferIndex == 42);
	}

	WHEN("Testing move assignation")
	{
		Buffer foo(42);
		Buffer bar;

		bar = std::move(foo);

		CHECK(foo.bufferIndex == -1);
		CHECK(bar.bufferIndex == 42);
	}
}

namespace
{
	struct Foo
	{
		Nz::MovableValue<CopyCounter> copyCounter;
	};
}

SCENARIO("MovableValue", "[MovableValue]")
{
	WHEN("Testing copy constructor")
	{
		Foo foo;
		Foo bar(foo);

		CHECK(bar.copyCounter.Get().GetCopyCount() == 1);
		CHECK(bar.copyCounter.Get().GetMoveCount() == 0);
	}

	WHEN("Testing move constructor")
	{
		Foo foo;
		Foo bar(std::move(foo));

		CHECK(bar.copyCounter.Get().GetCopyCount() == 0);
		CHECK(bar.copyCounter.Get().GetMoveCount() == 1);
	}

	WHEN("Testing copy assignation")
	{
		Foo foo;
		Foo bar;

		bar = foo;

		CHECK(bar.copyCounter.Get().GetCopyCount() == 1);
		CHECK(bar.copyCounter.Get().GetMoveCount() == 0);
	}

	WHEN("Testing move assignation")
	{
		Foo foo;
		Foo bar;

		bar = std::move(foo);

		CHECK(bar.copyCounter.Get().GetCopyCount() == 0);
		CHECK(bar.copyCounter.Get().GetMoveCount() == 1);
	}
}
