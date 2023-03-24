#pragma once

namespace raymino
{
struct XY
{
	unsigned x;
	unsigned y;
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