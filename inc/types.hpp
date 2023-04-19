#pragma once

#include <cstddef>
#include <tuple>

namespace raymino
{
struct XY
{
	int x;
	int y;
	XY operator+(XY other) const noexcept
	{
		return {x + other.x, y + other.y};
	}
	XY& operator+=(XY other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	constexpr bool operator==(XY other) const noexcept
	{
		return std::tie(x, y) == std::tie(other.x, other.y);
	}
};
struct Size
{
	int width;
	int height;
	[[nodiscard]] ptrdiff_t area() const noexcept
	{
		return static_cast<ptrdiff_t>(width) * height;
	}
	constexpr bool operator==(Size other) const noexcept
	{
		return std::tie(width, height) == std::tie(other.width, other.height);
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

enum class TetrominoType
{
	I,
	L,
	J,
	O,
	Z,
	T,
	S,
};
enum class RotationSystem
{
	Original,
	Super,
	Arika,
	Sega,
	Nintendo,
};
} // namespace raymino