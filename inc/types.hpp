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
	constexpr bool operator==(Rect other) const noexcept
	{
		return std::tie(static_cast<const XY&>(*this), static_cast<const Size&>(*this)) ==
		       std::tie(static_cast<const XY&>(other), static_cast<const Size&>(other));
	}
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
	NintendoLeft,
	NintendoRight,
};
enum class WallKicks
{
	None,
	Arika,
	Super,
};
enum class LockDown
{
	Infinit,
	Extended,
	Classic,
	Entry,
};
enum class SoftDrop
{
	Locking,
	NonLocking,
};
enum class InstantDrop
{
	Hard,
	Sonic,
};
enum class ShuffleType
{
	Random,
	SingleBag,
	DoubleBag,
	TripleBag,
};
enum class ScoringSystem
{
	Guideline,
	BPS,
	Sega,
	Nintendo,
};
enum class ScoreEvent
{
	LineClear,
	PerfectClear, // separat from *Clear & *Spin events
	MiniTSpin,
	TSpin,
	SoftDrop,
	HardDrop,
};
} // namespace raymino