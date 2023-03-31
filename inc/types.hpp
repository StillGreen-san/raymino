#pragma once

#include <cstddef>

namespace raymino
{
struct XY
{
	int x;
	int y;
	XY& operator+=(XY other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}
};
struct Size
{
	unsigned width;
	unsigned height;
	[[nodiscard]] size_t area() const noexcept
	{
		return width * height;
	}
};
struct Rect : XY, Size
{
};
template<typename TIterator>
struct Range
{
	TIterator first;
	TIterator last;
	[[nodiscard]] auto begin()
	{
		return first;
	}
	[[nodiscard]] auto end()
	{
		return last;
	}
	[[nodiscard]] auto begin() const
	{
		return first;
	}
	[[nodiscard]] auto end() const
	{
		return last;
	}
};
} // namespace raymino