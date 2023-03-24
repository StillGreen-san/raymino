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
};
struct Rect : XY, Size
{
};
} // namespace raymino