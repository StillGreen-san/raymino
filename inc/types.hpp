#pragma once

#include <cstddef>

namespace raymino
{
struct XY
{
	int x;
	int y;
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
} // namespace raymino